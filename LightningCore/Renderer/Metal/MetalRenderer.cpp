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
#include "SubTexture.h"
#include "GLFW/glfw3.h"
#include <print>

MetalRenderer::MetalRenderer(MTL::Device* p_MetalDevice, CA::MetalLayer* p_MetalLayer)
: m_MetalDevice(p_MetalDevice),
  m_MetalLayer(p_MetalLayer),
  m_MetalCommandQueue(m_MetalDevice->newCommandQueue()),
  m_DepthStencilDescriptor(MTL::DepthStencilDescriptor::alloc()->init()),
  m_Shader("Assets/Shaders/Shader2D.metal", m_MetalDevice, m_MetalLayer->pixelFormat())
{
    assert(m_MetalDevice);
    m_DepthStencilDescriptor->setDepthCompareFunction(MTL::CompareFunction::CompareFunctionLess);
    m_DepthStencilDescriptor->setDepthWriteEnabled(true);
    m_DepthStencilState = m_MetalDevice->newDepthStencilState(m_DepthStencilDescriptor);
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
    
    // TODO: Refactor this for better clarity
    for (auto &mesh: m_Meshes)
    {
        mesh.m_IndexBuffer->release();
        mesh.m_IndexBuffer = nullptr;
        
        mesh.m_VertexBuffer->release();
        mesh.m_VertexBuffer = nullptr;
        
        delete mesh.m_Texture;
        mesh.m_Texture = nullptr;
    }
  
    m_Meshes.clear();
    
    for (auto &mesh: m_3DMeshes)
    {
        mesh.m_IndexBuffer->release();
        mesh.m_IndexBuffer = nullptr;
        
        mesh.m_VertexBuffer->release();
        mesh.m_VertexBuffer = nullptr;
        
        delete mesh.m_Texture;
        mesh.m_Texture = nullptr;
    }
    m_3DMeshes.clear();
}


void MetalRenderer::CreateQuad(const char* p_FilePath, const simd::float3 &position)
{
    m_Mesh = m_MeshBuilder.GenerateQuadWithTexture(m_MetalDevice, p_FilePath);
    m_Mesh.m_Transform = matrix4x4_translation(position);
    m_Meshes.push_back(m_Mesh);
}

void MetalRenderer::CreateQuad(const char* p_FilePath, const simd::float3 &scale, const simd::float3 &position)
{
    m_Mesh = m_MeshBuilder.GenerateQuadWithTexture(m_MetalDevice, p_FilePath);
    m_Mesh.m_Transform = matrix4x4_scale_translation(scale, position);
    m_Meshes.push_back(m_Mesh);
}

void MetalRenderer::CreateQuad(const simd::float2 &position, const simd::float2 &size, const char* p_FilePath)
{
    m_Mesh = m_MeshBuilder.GenerateQuadWithTexture(m_MetalDevice, p_FilePath);
    m_Mesh.m_Transform = matrix4x4_translation(simd_make_float3(0.f, 0.f, -2.f));
    SubTexture::CreateFromCoords(m_Mesh.m_Texture->GetTexture(), position, size);
    m_Meshes.push_back(m_Mesh);
}

void MetalRenderer::CreateSprite(const char* p_FilePath, const simd::float3 &scale, const simd::float3 &position, const Sprite &sprite)
{
}

void MetalRenderer::CreateCube(const char* p_FilePath)
{
    m_Mesh3D = m_MeshBuilder.GenerateCube(m_MetalDevice, p_FilePath);
    m_3DMeshes.push_back(m_Mesh3D);
}

void MetalRenderer::BeginFrame()
{
    m_MetalCommandBuffer = m_MetalCommandQueue->commandBuffer();
}

void MetalRenderer::Render()
{
    
    MTL::RenderPassDepthAttachmentDescriptor* depthAttachment = m_RenderPassDescriptor->depthAttachment();
    depthAttachment->setClearDepth(1.0f);
    m_RenderCommandEncoder = m_MetalCommandBuffer->renderCommandEncoder(m_RenderPassDescriptor);
    m_RenderCommandEncoder->setRenderPipelineState(m_Shader.GetRenderPipelineState());
    m_RenderCommandEncoder->setDepthStencilState(m_DepthStencilState);
    matrix_float4x4 view = m_Camera.GetViewMatrix();
    m_Shader.SetVertexShaderUniformMatrix4x4(m_RenderCommandEncoder, view, 3);
    
    matrix_float4x4 projection = matrix_perspective_right_hand(90.0f * (M_PI / 180.f),
                                                               m_MetalLayer->drawableSize().width / m_MetalLayer->drawableSize().height,
                                                               1.f,
                                                               1000.f);
    m_Shader.SetVertexShaderUniformMatrix4x4(m_RenderCommandEncoder, projection, 2);
    
    for (auto &mesh : m_Meshes)
    {
        //SpriteUniform spriteUniform = mesh.m_Sprite.GetUniforms();
        
        m_RenderCommandEncoder->setVertexBytes(&mesh.m_Transform, sizeof(matrix_float4x4), 1);
        m_Shader.SetVertexShaderUniformMatrix4x4(m_RenderCommandEncoder, mesh.m_Transform, 1);
        m_RenderCommandEncoder->setVertexBuffer(mesh.m_VertexBuffer, 0, 0);
        m_RenderCommandEncoder->setFragmentTexture(mesh.m_Texture->GetTexture(), 0);
        //m_RenderCommandEncoder->setVertexBytes(&spriteUniform, sizeof(SpriteUniform), 4);
        m_RenderCommandEncoder->drawIndexedPrimitives(MTL::PrimitiveTypeTriangle,
                                                      NS::UInteger(4), MTL::IndexType::IndexTypeUInt16,
                                                      mesh.m_IndexBuffer,
                                                      NS::UInteger(0));
    }
    
    float angleInDegrees = glfwGetTime() * 0.5f * 90;
    float angleInRadians = angleInDegrees * M_PI / 180.0f;
    
    matrix_float4x4 translationMatrix = matrix4x4_translation(simd::make_float3(0.0f, 0.0f, 0.0f));

    matrix_float4x4 rotationMatrix = matrix4x4_rotation(angleInRadians, simd::make_float3(0.0, -1.0, 0.0));

    
    for (auto &mesh : m_3DMeshes)
    {
        mesh.m_Transform = simd_mul(translationMatrix, rotationMatrix);
        m_RenderCommandEncoder->setFrontFacingWinding(MTL::WindingCounterClockwise);
        m_RenderCommandEncoder->setCullMode(MTL::CullModeBack);
        m_RenderCommandEncoder->setVertexBytes(&mesh.m_Transform, sizeof(matrix_float4x4), 1);
        m_Shader.SetVertexShaderUniformMatrix4x4(m_RenderCommandEncoder, mesh.m_Transform, 1);
        m_RenderCommandEncoder->setVertexBuffer(mesh.m_VertexBuffer, 0, 0);
        m_RenderCommandEncoder->setFragmentTexture(mesh.m_Texture->GetTexture(), 0);
        m_RenderCommandEncoder->drawIndexedPrimitives(MTL::PrimitiveTypeTriangle,
                                                      36, MTL::IndexType::IndexTypeUInt16,
                                                      mesh.m_IndexBuffer,
                                                      NS::UInteger(0));
    }
    
    
    m_RenderCommandEncoder->endEncoding();

}

void MetalRenderer::Commit()
{
    m_MetalCommandBuffer->commit();
}


