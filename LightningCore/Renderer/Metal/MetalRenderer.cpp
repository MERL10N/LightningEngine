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
#include <print>

MetalRenderer::MetalRenderer(MTL::Device* p_MetalDevice, CA::MetalLayer* p_MetalLayer)
: m_MetalDevice(p_MetalDevice),
  m_MetalLayer(p_MetalLayer),
  m_MetalCommandQueue(m_MetalDevice->newCommandQueue()),
  m_DepthStencilDescriptor(MTL::DepthStencilDescriptor::alloc()->init()),
  m_Shader("Assets/Shaders/Shader.metal", m_MetalDevice, m_MetalLayer->pixelFormat())
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

// TODO: This function is still work in progress
void MetalRenderer::CreateQuad(const simd::float2 &position, const simd::float2 &size, const char* p_FilePath)
{
    m_Mesh = m_MeshBuilder.GenerateQuadWithTexture(m_MetalDevice, p_FilePath);
    m_Mesh.m_Transform = matrix4x4_translation(simd_make_float3(0.f, 0.f, -2.f));
    SubTexture::CreateFromCoords(m_Mesh.m_Texture->GetTexture(), position, size);
    m_Meshes.push_back(m_Mesh);
}

void MetalRenderer::CreateCube(const char* p_FilePath)
{
    m_Mesh = m_MeshBuilder.GenerateCube(m_MetalDevice, p_FilePath);
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
    m_RenderCommandEncoder->setVertexBytes(&view, sizeof(matrix_float4x4), 3);
    
    // TODO: Switch to orthographic camera for 2D Game
    matrix_float4x4 projection = matrix_perspective_right_hand(90.0f * (M_PI / 180.f),
                                                               m_MetalLayer->drawableSize().width / m_MetalLayer->drawableSize().height,
                                                               1.f,
                                                               1000.f);
    m_RenderCommandEncoder->setVertexBytes(&projection, sizeof(matrix_float4x4), 2);
    
    for (auto &mesh : m_Meshes)
    {
        m_RenderCommandEncoder->setVertexBytes(&mesh.m_Transform, sizeof(matrix_float4x4), 1);
        m_RenderCommandEncoder->setVertexBuffer(mesh.m_VertexBuffer, 0, 0);
        m_RenderCommandEncoder->setFragmentTexture(mesh.m_Texture->GetTexture(), 0);
        m_RenderCommandEncoder->drawIndexedPrimitives(MTL::PrimitiveTypeTriangleStrip,
                                                      NS::UInteger(4), MTL::IndexType::IndexTypeUInt16,
                                                      mesh.m_IndexBuffer,
                                                      NS::UInteger(0));
    }
    
    m_RenderCommandEncoder->endEncoding();

}

void MetalRenderer::Commit()
{
    m_MetalCommandBuffer->commit();
}


