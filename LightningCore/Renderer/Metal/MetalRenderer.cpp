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
#include "MetalVertexDescriptor.h"
#include "Primitives/Sprite.h"
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
  m_MetalCommandQueue(m_MetalDevice->newMTL4CommandQueue()),
  m_MetalCommandBuffer(m_MetalDevice->newCommandBuffer()),
  m_ArgumentTableDescriptor(MTL4::ArgumentTableDescriptor::alloc()->init()),
  m_ResidencySetDescriptor(MTL::ResidencySetDescriptor::alloc()->init()),
  m_DepthStencilDescriptor(MTL::DepthStencilDescriptor::alloc()->init()),
  b_EnableWireframe(false),
  m_LightComponent(LightComponent())
{
    assert(m_MetalDevice);
    m_ArgumentTableDescriptor->setMaxBufferBindCount(1);
    m_ArgumentTable = m_MetalDevice->newArgumentTable(m_ArgumentTableDescriptor, nullptr);
    
    if (m_ArgumentTableDescriptor)
    {
        m_ArgumentTableDescriptor->release();
        m_ArgumentTableDescriptor = nullptr;
    }
    
    m_ResidencySet = m_MetalDevice->newResidencySet(m_ResidencySetDescriptor, nullptr);
    
    if (m_ResidencySetDescriptor)
    {
        m_ResidencySetDescriptor->release();
        m_ResidencySetDescriptor = nullptr;
    }
    
    m_ResidencySet->commit();
    m_MetalCommandQueue->addResidencySet(m_ResidencySet);
    m_MetalCommandQueue->addResidencySet(m_MetalLayer->residencySet());
    
    m_DepthStencilDescriptor->setDepthCompareFunction(MTL::CompareFunctionLess);
    m_DepthStencilDescriptor->setDepthWriteEnabled(true);
    m_DepthStencilState = m_MetalDevice->newDepthStencilState(m_DepthStencilDescriptor);

    if (m_DepthStencilDescriptor)
    {
        m_DepthStencilDescriptor->release();
        m_DepthStencilDescriptor = nullptr;
    }
    
    MetalVertexDescriptor vertexDescriptorBuilder;
    
    m_3DVertexDescriptor = vertexDescriptorBuilder
        .AddAttribute(MTL::VertexFormatFloat3, offsetof(Vertex3D, pos))
        .AddAttribute(MTL::VertexFormatFloat3, offsetof(Vertex3D, color))
        .AddAttribute(MTL::VertexFormatFloat3, offsetof(Vertex3D, normals))
        .AddAttribute(MTL::VertexFormatFloat2, offsetof(Vertex3D, texCoord))
        .SetBufferLayout(sizeof(Vertex3D))
        .BuildVertexDescriptor();
    
    
    m_LightVertexDescriptor = vertexDescriptorBuilder
        .AddAttribute(MTL::VertexFormatFloat3, offsetof(Vertex3D, pos))
        .AddAttribute(MTL::VertexFormatFloat3, offsetof(Vertex3D, color))
        .AddAttribute(MTL::VertexFormatFloat3, offsetof(Vertex3D, normals))
        .SetBufferLayout(sizeof(Vertex3D))
        .BuildVertexDescriptor();
    
    m_TextureShader = new MetalShader("Assets/Shaders/Shader.metal", "vertex_main", "fragment_main", m_MetalDevice, m_3DVertexDescriptor, m_MetalLayer->pixelFormat());
    
    m_UntexturedShader = new MetalShader("Assets/Shaders/Shader.metal", "vertex_main", "fragment_main_untextured", m_MetalDevice, m_3DVertexDescriptor, m_MetalLayer->pixelFormat());
    
    m_LightShader = new MetalShader("Assets/Shaders/Light.metal", "vertex_light", "fragment_light", m_MetalDevice, m_LightVertexDescriptor, m_MetalLayer->pixelFormat());
    
    if (m_3DVertexDescriptor)
    {
        m_3DVertexDescriptor->release();
        m_3DVertexDescriptor = nullptr;
    }
    if (m_LightVertexDescriptor)
    {
        m_LightVertexDescriptor->release();
        m_LightVertexDescriptor = nullptr;
    }
}

MetalRenderer::~MetalRenderer()
{
    
    if (m_LightShader)
    {
        delete m_LightShader;
        m_LightShader = nullptr;
    }
    
    if (m_TextureShader)
    {
        delete m_TextureShader;
        m_TextureShader = nullptr;
    }
    
    if (m_UntexturedShader)
    {
        delete m_UntexturedShader;
        m_UntexturedShader = nullptr;
    }
    
    if (m_MetalCommandQueue)
    {
        m_MetalCommandQueue->release();
        m_MetalCommandQueue = nullptr;
    }
    
    if (m_MetalLayer)
    {
        m_MetalLayer->release();
        m_MetalLayer = nullptr;
    }
    
    if (m_MetalDevice)
    {
        m_MetalDevice->release();
        m_MetalDevice = nullptr;
    }
}

void MetalRenderer::SubmitCommandBuffer()
{
    //m_MetalCommandBuffer = m_MetalCommandQueue->commandBuffer();
}


void MetalRenderer::BeginScene(const Camera &p_Camera, const float p_AspectRatio)
{
    m_Camera = p_Camera;
    //m_MetalCommandBuffer = m_MetalCommandQueue->commandBuffer();
    
    MTL::RenderPassDepthAttachmentDescriptor* depthAttachment = m_RenderPassDescriptor->depthAttachment();
    depthAttachment->setClearDepth(1.0f);
    
    m_RenderCommandEncoder = m_MetalCommandBuffer->renderCommandEncoder(m_RenderPassDescriptor);
    m_RenderCommandEncoder->setDepthStencilState(m_DepthStencilState);
    m_RenderCommandEncoder->setFrontFacingWinding(MTL::WindingCounterClockwise);
    m_RenderCommandEncoder->setCullMode(MTL::CullModeBack);
    
    if (b_EnableWireframe)
    {
        m_RenderCommandEncoder->setTriangleFillMode(MTL::TriangleFillModeLines);
    }
    else
    {
        m_RenderCommandEncoder->setTriangleFillMode(MTL::TriangleFillModeFill);
    }
    
    m_ViewMatrix = m_Camera.GetViewMatrix();
    
    float fov = m_Camera.GetZoom() * (M_PI / 180.0f);
    
    m_PerspectiveMatrix = matrix_perspective_right_hand(fov,
                                                       p_AspectRatio,
                                                       0.1f,
                                                       1000.f);
    

}

void MetalRenderer::RenderLights(const matrix_float4x4 &p_ModelMatrix, const Mesh_3D& p_3DMesh, const LightComponent &p_LightComponent)
{
    m_LightComponent = p_LightComponent;
    
    m_LightComponent.m_Position = simd::make_float3(p_ModelMatrix.columns[3].x,
                                                    p_ModelMatrix.columns[3].y,
                                                    p_ModelMatrix.columns[3].z);
    
    //m_RenderCommandEncoder->setVertexBuffer(p_3DMesh.m_VertexBuffer, 0, 0);
    m_ResidencySet->addAllocation(p_3DMesh.m_VertexBuffer);
    m_ArgumentTable->setAddress(p_3DMesh.m_VertexBuffer->gpuAddress(), 0);
    
    if (m_LightShader)
    {
        m_ModelMatrix = p_ModelMatrix;
        m_RenderCommandEncoder->setRenderPipelineState(m_LightShader->GetRenderPipelineState());
        m_LightShader->SetVertexShaderUniform(m_RenderCommandEncoder, m_PerspectiveMatrix, 1);
        m_LightShader->SetVertexShaderUniform(m_RenderCommandEncoder, m_ViewMatrix, 2);
        m_LightShader->SetVertexShaderUniform(m_RenderCommandEncoder, p_ModelMatrix, 3);
        m_LightShader->SetFragmentShaderUniform(m_RenderCommandEncoder, p_LightComponent.m_Color, 0);
    }
    
    
    m_RenderCommandEncoder->setArgumentTable(m_ArgumentTable, MTL::RenderStageVertex);
    m_RenderCommandEncoder->drawIndexedPrimitives(MTL::PrimitiveTypeTriangle,
                                                 p_3DMesh.m_IndexCount, MTL::IndexType::IndexTypeUInt16,
                                                 (uint64_t)p_3DMesh.m_IndexBuffer,
                                                 NS::UInteger(0));
}

void MetalRenderer::RenderMesh(const matrix_float4x4& p_ModelMatrix, const Mesh_3D& p_3DMesh, const MetalTexture* p_Texture)
{
       //m_RenderCommandEncoder->setVertexBuffer(p_3DMesh.m_VertexBuffer, 0, 0);
        if (p_Texture)
        {
            m_ModelMatrix = p_ModelMatrix;
            //m_ArgumentBuffer = m_TextureShader->InitialiseArgumentBuffers(p_Texture->GetTexture());
           // m_RenderCommandEncoder->setFragmentBuffer(m_ArgumentBuffer, 0, 0);
           // m_RenderCommandEncoder->useResource(p_Texture->GetTexture(), MTL::ResourceUsageRead, MTL::RenderStageFragment);
            m_RenderCommandEncoder->setRenderPipelineState(m_TextureShader->GetRenderPipelineState());
            m_TextureShader->SetVertexShaderUniform(m_RenderCommandEncoder, m_PerspectiveMatrix, 1);
            m_TextureShader->SetVertexShaderUniform(m_RenderCommandEncoder, m_ViewMatrix, 2);
            m_TextureShader->SetVertexShaderUniform(m_RenderCommandEncoder, p_ModelMatrix, 3);
            m_TextureShader->SetFragmentShaderUniform(m_RenderCommandEncoder, m_LightComponent.m_Color, 1); // Light color
            m_TextureShader->SetFragmentShaderUniform(m_RenderCommandEncoder, m_LightComponent.m_Position, 2); // Light position
            m_TextureShader->SetFragmentShaderUniform(m_RenderCommandEncoder, m_Camera.GetPosition(), 3);
        }
        else
        {
            
            m_RenderCommandEncoder->setRenderPipelineState(m_UntexturedShader->GetRenderPipelineState());
            m_UntexturedShader->SetVertexShaderUniform(m_RenderCommandEncoder, m_PerspectiveMatrix, 1);
            m_UntexturedShader->SetVertexShaderUniform(m_RenderCommandEncoder, m_ViewMatrix, 2);
            m_UntexturedShader->SetVertexShaderUniform(m_RenderCommandEncoder, p_ModelMatrix, 3);
            m_UntexturedShader->SetFragmentShaderUniform(m_RenderCommandEncoder, m_LightComponent.m_Color, 0); // Light color
            m_UntexturedShader->SetFragmentShaderUniform(m_RenderCommandEncoder, m_LightComponent.m_Position, 1); // Light position
            m_UntexturedShader->SetFragmentShaderUniform(m_RenderCommandEncoder, m_Camera.GetPosition(), 2);
        }
    
        m_RenderCommandEncoder->setArgumentTable(m_ArgumentTable, MTL::RenderStageVertex);
        m_RenderCommandEncoder->drawIndexedPrimitives(MTL::PrimitiveTypeTriangle,
                                                      p_3DMesh.m_IndexCount, MTL::IndexType::IndexTypeUInt16,
                                                      (uint64_t)p_3DMesh.m_IndexBuffer,
                                                      NS::UInteger(0));
      

}

void MetalRenderer::EndScene(MTL::Drawable* p_MetalDrawable)
{
    m_RenderCommandEncoder->endEncoding();
    //m_MetalCommandBuffer->commit();
    m_MetalCommandQueue->wait(p_MetalDrawable);
    m_MetalCommandQueue->commit(&m_MetalCommandBuffer, 1);
    m_MetalCommandQueue->signalDrawable(p_MetalDrawable);
    p_MetalDrawable->present();
}
