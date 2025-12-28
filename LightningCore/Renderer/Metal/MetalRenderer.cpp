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
#include "Math/AAPLMathUtilities.h"

MetalRenderer::MetalRenderer(MTL::Device* p_MetalDevice, CA::MetalLayer* p_MetalLayer)
: m_MetalDevice(p_MetalDevice),
  m_MetalLayer(p_MetalLayer),
  m_MetalCommandQueue(m_MetalDevice->newCommandQueue()),
  m_Shader("Assets/Shaders/Shader.metal", p_MetalDevice, p_MetalLayer->pixelFormat()),
  m_Camera()
{
    assert(m_MetalDevice);
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
    
    m_QuadMesh.indexBuffer->release();
    m_QuadMesh.vertexBuffer->release();
}

void MetalRenderer::CreateQuad(const char* p_FilePath)
{
    m_QuadMesh = m_MeshBuilder.GenerateQuad(m_MetalDevice, p_FilePath);
}

void MetalRenderer::CreateCube(const char* p_FilePath)
{
    m_CubeMesh = m_MeshBuilder.GenerateCube(m_MetalDevice, p_FilePath);
}

void MetalRenderer::BeginFrame()
{
    m_MetalCommandBuffer = m_MetalCommandQueue->commandBuffer();
}

void MetalRenderer::Render()
{
    m_RenderCommandEncoder = m_MetalCommandBuffer->renderCommandEncoder(m_RenderPassDescriptor);
    m_RenderCommandEncoder->setRenderPipelineState(m_Shader.GetRenderPipelineState());
    
    matrix_float4x4 view = m_Camera.GetViewMatrix();
    m_RenderCommandEncoder->setVertexBytes(&view, sizeof(matrix_float4x4), 3);
    matrix_float4x4 projection = matrix_perspective_right_hand(45.0f, m_MetalLayer->drawableSize().width / m_MetalLayer->drawableSize().height, 0.2f, 10.f);
    m_RenderCommandEncoder->setVertexBytes(&projection, sizeof(matrix_float4x4), 2);
    matrix_float4x4 transform = matrix4x4_translation(0.0f, 0.0f, -4.0f);
    m_RenderCommandEncoder->setVertexBytes(&transform, sizeof(matrix_float4x4), 1);
    m_RenderCommandEncoder->setVertexBuffer(m_QuadMesh.vertexBuffer, 0, 0);
    m_RenderCommandEncoder->setFragmentTexture(m_QuadMesh.texture->GetTexture(), 0);
    m_RenderCommandEncoder->drawIndexedPrimitives(MTL::PrimitiveTypeTriangleStrip, NS::UInteger(4), MTL::IndexType::IndexTypeUInt16, m_QuadMesh.indexBuffer, NS::UInteger(0));
    m_RenderCommandEncoder->endEncoding();

}

void MetalRenderer::Commit()
{
    m_MetalCommandBuffer->commit();
}
