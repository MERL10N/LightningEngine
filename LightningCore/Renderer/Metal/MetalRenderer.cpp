//
// Created by Kian Marvi on 11/24/24.
//

#include "MetalRenderer.h"
#include <Metal/Metal.hpp>
#include <MetalKit/MetalKit.hpp>
#include "MeshBuilder.h"
#include "MetalBuffer.h"
#include "MetalTexture.h"

MetalRenderer::MetalRenderer(MTL::Device* p_MetalDevice)
: m_MetalDevice(p_MetalDevice),
  m_MetalCommandQueue(m_MetalDevice->newCommandQueue()),
  m_MetalCommandBuffer(nullptr),
  m_RenderPassDescriptor(nullptr),
  m_RenderCommandEncoder(nullptr),
  m_Shader("../LightningGame/Shaders/Shader.metal", m_MetalDevice),
  m_VertexBuffer(new MetalVertexBuffer(m_MetalDevice)),
  m_Texture(new MetalTexture("../LightningGame/Assets/megaman.png", m_MetalDevice))
{
    assert(m_MetalDevice);
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
    
    
    if (m_Texture)
    {
        delete m_Texture;
        m_Texture = nullptr;
    }
}

void MetalRenderer::CreateTriangle()
{
    MeshBuilder::GenerateQuad(m_MetalDevice, m_VertexBuffer);
}


void MetalRenderer::Render(MTK::View* p_MetalKitView)
{
    m_MetalCommandBuffer = m_MetalCommandQueue->commandBuffer();
    m_RenderPassDescriptor = p_MetalKitView->currentRenderPassDescriptor();
    m_RenderCommandEncoder = m_MetalCommandBuffer->renderCommandEncoder(m_RenderPassDescriptor);

    assert(m_RenderCommandEncoder);
    assert(m_MetalCommandQueue);
    assert(m_RenderPassDescriptor);
    assert(m_MetalCommandBuffer);
    assert(m_RenderCommandEncoder);
    assert(m_Shader.GetRenderPipelineState());
    
    m_RenderCommandEncoder->setFrontFacingWinding(MTL::WindingCounterClockwise);
    m_RenderCommandEncoder->setRenderPipelineState(m_Shader.GetRenderPipelineState());
    m_RenderCommandEncoder->setFragmentTexture(m_Texture->GetTexture(), 0);
    m_RenderCommandEncoder->setVertexBuffer(m_VertexBuffer->GetVertexBuffer(), 0, 0);
    
    m_RenderCommandEncoder->drawPrimitives(MTL::PrimitiveTypeTriangleStrip, NS::UInteger(0), NS::UInteger(4));
    m_RenderCommandEncoder->endEncoding();
    
    m_MetalCommandBuffer->presentDrawable(p_MetalKitView->currentDrawable());
    m_MetalCommandBuffer->commit();
}
