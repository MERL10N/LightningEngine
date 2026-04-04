//
// Created by Kian Marvi on 11/24/24.
//

#include "MetalRenderer.h"
#include "Metal/Metal.hpp"
#include "QuartzCore/CAMetalLayer.hpp"
#include "MeshBuilder.h"
#include "MetalTexture.h"
#include "MetalBuffer.h"
#include "Primitives/MeshBuilder.h"
#include "MetalVertexDescriptorBuilder.h"
#include "MetalShader.h"
#include "SubTexture.h"
#include "GLFW/glfw3.h"
#include "Scene/Scene.h"
#include "Scene/Component.h"
#include <print>
#include <cstddef>

MetalRenderer::MetalRenderer(MTL::Device* p_MetalDevice, CA::MetalLayer* p_MetalLayer)
: m_MetalDevice(p_MetalDevice),
  m_MetalLayer(p_MetalLayer),
  m_MetalCommandQueue(m_MetalDevice->newCommandQueue())
{
    assert(m_MetalDevice);
    m_DepthStencilDescriptor = MTL::DepthStencilDescriptor::alloc()->init();
    m_DepthStencilDescriptor->setDepthCompareFunction(MTL::CompareFunctionLess);
    m_DepthStencilDescriptor->setDepthWriteEnabled(true);
    m_DepthStencilState = m_MetalDevice->newDepthStencilState(m_DepthStencilDescriptor);
    m_DepthStencilDescriptor->release();
    
    MetalVertexDescriptorBuilder vertexDescriptorBuilder;
    
    m_3DVertexDescriptor = vertexDescriptorBuilder
        .AddAttribute(MTL::VertexFormatFloat3, offsetof(Vertex3D, pos))
        .AddAttribute(MTL::VertexFormatFloat3, offsetof(Vertex3D, color))
        .AddAttribute(MTL::VertexFormatFloat3, offsetof(Vertex3D, normals))
        .AddAttribute(MTL::VertexFormatFloat2, offsetof(Vertex3D, texCoord))
        .SetBufferLayout(sizeof(Vertex3D))
        .BuildVertexDescriptor();
    
   // m_LightVertexDescriptor = vertexDescriptorBuilder.AddAttribute(MTL::VertexFormatFloat4, offsetof(simd::float4x4, pos));
    
    m_Shader = new MetalShader("Assets/Shaders/Shader.metal", "vertex_main", "fragment_main", m_MetalDevice, m_3DVertexDescriptor, m_MetalLayer->pixelFormat());
    
    m_LightShader = new MetalShader("Assets/Shaders/Shader.metal", "vertex_main", "fragment_main_untextured", m_MetalDevice, m_3DVertexDescriptor, m_MetalLayer->pixelFormat());
    
    m_3DVertexDescriptor->release();
    
}

MetalRenderer::~MetalRenderer()
{
    if (m_MetalDevice)
    {
        m_MetalDevice->release();
        m_MetalDevice = nullptr;
    }
    
    if (m_MetalLayer)
    {
        m_MetalLayer->release();
        m_MetalLayer = nullptr;
    }

    if (m_MetalCommandQueue)
    {
        m_MetalCommandQueue->release();
        m_MetalCommandQueue = nullptr;
    }
    
    if (m_DepthStencilDescriptor)
    {
        m_DepthStencilDescriptor->release();
        m_DepthStencilDescriptor = nullptr;
    }
    
    if (m_LightShader)
    {
        delete m_LightShader;
        m_LightShader = nullptr;
    }
    
    if (m_Shader)
    {
        delete m_Shader;
        m_Shader = nullptr;
    }
    
    // TODO: Refactor this for better clarity
    for (auto &mesh: m_2DMeshes)
    {
        mesh.m_IndexBuffer->release();
        mesh.m_IndexBuffer = nullptr;
        
        mesh.m_VertexBuffer->release();
        mesh.m_VertexBuffer = nullptr;
        
        delete mesh.m_Texture;
        mesh.m_Texture = nullptr;
    }
  
    m_2DMeshes.clear();
    
    for (auto &mesh: m_3DMeshes)
    {
        mesh.m_IndexBuffer->release();
        mesh.m_IndexBuffer = nullptr;
        
        mesh.m_VertexBuffer->release();
        mesh.m_VertexBuffer = nullptr;
        
        if (mesh.m_Texture)
        {
            delete mesh.m_Texture;
            mesh.m_Texture = nullptr;
        }
    }
    m_3DMeshes.clear();
}


void MetalRenderer::CreateQuad(const char* p_FilePath, const simd::float3 &position)
{
    m_Mesh = m_MeshBuilder.GenerateQuadWithTexture(m_MetalDevice, p_FilePath);
    m_Mesh.m_Transform = matrix4x4_translation(position);
    m_2DMeshes.push_back(m_Mesh);
}

void MetalRenderer::CreateQuad(const char* p_FilePath, const simd::float3 &scale, const simd::float3 &position)
{
    m_Mesh = m_MeshBuilder.GenerateQuadWithTexture(m_MetalDevice, p_FilePath);
    m_Mesh.m_Transform = matrix4x4_scale_translation(scale, position);
    m_2DMeshes.push_back(m_Mesh);
}

void MetalRenderer::CreateQuad(const simd::float2 &position, const simd::float2 &size, const char* p_FilePath)
{
    m_Mesh = m_MeshBuilder.GenerateQuadWithTexture(m_MetalDevice, p_FilePath);
    m_Mesh.m_Transform = matrix4x4_translation(simd_make_float3(0.f, 0.f, -2.f));
    SubTexture::CreateFromCoords(m_Mesh.m_Texture->GetTexture(), position, size);
    m_2DMeshes.push_back(m_Mesh);
}

void MetalRenderer::CreateSprite(const char* p_FilePath, const simd::float3 &scale, const simd::float3 &position, const Sprite &sprite)
{
}

void MetalRenderer::CreateCube(const char* p_FilePath)
{
    m_Mesh3D = m_MeshBuilder.GenerateCube(m_MetalDevice, p_FilePath);
    m_Mesh3D.m_Transform = matrix4x4_translation(simd::make_float3(0.0f, 0.0f, 0.0f));
    m_3DMeshes.push_back(m_Mesh3D);
}

void MetalRenderer::CreateSphere()
{
    m_Mesh3D = m_MeshBuilder.GenerateSphere(m_MetalDevice, 34, 34);
    m_Mesh3D.m_Transform = matrix4x4_translation(simd_make_float3(-2.f, 0.f, 0.f));
    m_Mesh3D.m_Texture = nullptr;
    m_3DMeshes.push_back(m_Mesh3D);
}


void MetalRenderer::BeginFrame()
{
    m_MetalCommandBuffer = m_MetalCommandQueue->commandBuffer();
}

void MetalRenderer::Render(Scene* p_Scene)
{
    
    MTL::RenderPassDepthAttachmentDescriptor* depthAttachment = m_RenderPassDescriptor->depthAttachment();
    depthAttachment->setClearDepth(1.0f);
    m_RenderCommandEncoder = m_MetalCommandBuffer->renderCommandEncoder(m_RenderPassDescriptor);
    m_RenderCommandEncoder->setDepthStencilState(m_DepthStencilState);
    
    
    matrix_float4x4 viewMatrix = m_Camera.GetViewMatrix();
    
    float fov = m_Camera.GetZoom() * (M_PI / 180.0f);
    
    matrix_float4x4 projectionMatrix = matrix_perspective_right_hand(fov,
                                                               m_MetalLayer->drawableSize().width / m_MetalLayer->drawableSize().height,
                                                               0.1f,
                                                               1000.f);
    
    
    m_Shader->SetVertexShaderUniformMatrix4x4(m_RenderCommandEncoder, viewMatrix, 3);
    m_Shader->SetVertexShaderUniformMatrix4x4(m_RenderCommandEncoder, projectionMatrix, 2);
    
    auto view = p_Scene->Reg().view<TransformComponent, MeshComponent>();
    
    
    for (auto &entity : view)
    {
        
        auto& transformComponent = view.get<TransformComponent>(entity);
        auto& meshComponent = view.get<MeshComponent>(entity);
        
        Mesh_3D& mesh = meshComponent.m_Mesh;
        mesh.m_Transform = transformComponent.m_Transform;
        
        m_RenderCommandEncoder->setFrontFacingWinding(MTL::WindingCounterClockwise);
        m_RenderCommandEncoder->setCullMode(MTL::CullModeBack);
       
        m_RenderCommandEncoder->setVertexBuffer(mesh.m_VertexBuffer, 0, 0);
        if (mesh.m_Texture)
        {
            m_RenderCommandEncoder->setRenderPipelineState(m_Shader->GetRenderPipelineState());
            m_Shader->SetVertexShaderUniformMatrix4x4(m_RenderCommandEncoder, mesh.m_Transform, 1);
            m_RenderCommandEncoder->setFragmentTexture(mesh.m_Texture->GetTexture(), 0);
        }
        else
        {
            m_RenderCommandEncoder->setRenderPipelineState(m_LightShader->GetRenderPipelineState());
            m_LightShader->SetVertexShaderUniformMatrix4x4(m_RenderCommandEncoder, mesh.m_Transform, 1);
        }
        m_RenderCommandEncoder->drawIndexedPrimitives(MTL::PrimitiveTypeTriangle,
                                                      mesh.m_IndexCount, MTL::IndexType::IndexTypeUInt16,
                                                      mesh.m_IndexBuffer,
                                                      NS::UInteger(0));
    }
    
    
    m_RenderCommandEncoder->endEncoding();

}

void MetalRenderer::Commit()
{
    m_MetalCommandBuffer->commit();
}


