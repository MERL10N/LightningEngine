//
// Created by Kian Marvi on 11/24/24.
//

#include "MetalRenderer.h"
#include <Metal/Metal.hpp>
#include <MetalKit/MetalKit.hpp>
#include "MeshBuilder.h"
#include "MetalBuffer.h"

MetalRenderer::MetalRenderer(MTL::Device* p_metalDevice)
: m_MetalDevice(p_metalDevice),
  m_MetalCommandQueue(m_MetalDevice->newCommandQueue()),
  m_MetalCommandBuffer(nullptr),
  m_RenderPassDescriptor(nullptr),
  m_RenderCommandEncoder(nullptr),
  m_Shader("Shaders/Shader.metal", m_MetalDevice),
  m_Timer(Timer())
{
    CreateTriangle();
    
}

MetalRenderer::~MetalRenderer()
{
    if (m_MetalDevice)
    {
        m_MetalDevice->release();
        m_MetalDevice = nullptr;
    }

    if (m_MetalCommandQueue)
    {
        m_MetalCommandQueue->release();
        m_MetalCommandQueue = nullptr;
    }
    
    if (m_VertexBuffer)
    {
        delete m_VertexBuffer;
        m_VertexBuffer = nullptr;
    }
}

void MetalRenderer::CreateTriangle()
{
    m_VertexBuffer = MeshBuilder::GenerateTriangle(m_MetalDevice);
}


void MetalRenderer::Render(const MTK::View* p_MetalKitView)
{
    m_MetalCommandBuffer = m_MetalCommandQueue->commandBuffer();
    m_RenderPassDescriptor = p_MetalKitView->currentRenderPassDescriptor();
    m_RenderCommandEncoder = m_MetalCommandBuffer->renderCommandEncoder(m_RenderPassDescriptor);
    
    m_RenderCommandEncoder->setRenderPipelineState(m_Shader.GetRenderPipelineState());
    m_RenderCommandEncoder->setVertexBuffer(m_VertexBuffer->GetVertexBuffer(), 0, 0);

    m_RenderCommandEncoder->drawPrimitives(MTL::PrimitiveTypeTriangle, NS::UInteger{0}, NS::UInteger{3});
    
    m_RenderCommandEncoder->endEncoding();
    m_MetalCommandBuffer->presentDrawable(p_MetalKitView->currentDrawable());
    m_MetalCommandBuffer->commit();
}
