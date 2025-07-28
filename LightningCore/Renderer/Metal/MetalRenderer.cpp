//
// Created by Kian Marvi on 11/24/24.
//

#include "MetalRenderer.h"
#include <Metal/Metal.hpp>
#include <MetalKit/MetalKit.hpp>
#include "MeshBuilder.h"
#include "MetalBuffer.h"
#include "MetalTexture.h"

MetalRenderer::MetalRenderer(MTL::Device* p_MetalDevice, MTL::PixelFormat p_DepthAttachmentPixelFormat)
: m_MetalDevice(p_MetalDevice),
  m_MetalCommandQueue(m_MetalDevice->newCommandQueue()),
  m_MetalCommandBuffer(nullptr),
  m_RenderPassDescriptor(nullptr),
  m_RenderCommandEncoder(nullptr),
  m_Shader("../LightningGame/Shaders/Shader.metal", m_MetalDevice, p_DepthAttachmentPixelFormat),
  m_VertexBuffer(new MetalVertexBuffer(m_MetalDevice))
{
    assert(m_MetalDevice);
    CreateQuad("../LightningGame/Assets/megaman.png");
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
    
    if (m_MTKView)
    {
        m_MTKView->release();
        m_MTKView = nullptr;
    }
}

void MetalRenderer::CreateQuad(const char* p_TextureFilePath)
{
    m_Texture = new MetalTexture(p_TextureFilePath, m_MetalDevice);
    
    MeshBuilder::GenerateQuad(m_VertexBuffer);
}

void MetalRenderer::BeginFrame()
{
    m_MetalCommandBuffer = m_MetalCommandQueue->commandBuffer();
}

void MetalRenderer::Render(MTK::View* p_MetalKitView)
{
    m_MTKView = p_MetalKitView;
    m_RenderPassDescriptor = m_MTKView->currentRenderPassDescriptor();
    m_RenderCommandEncoder = m_MetalCommandBuffer->renderCommandEncoder(m_RenderPassDescriptor);
    m_RenderCommandEncoder->setFrontFacingWinding(MTL::WindingCounterClockwise);
    m_RenderCommandEncoder->setRenderPipelineState(m_Shader.GetRenderPipelineState());
    m_RenderCommandEncoder->setFragmentTexture(m_Texture->GetTexture(), 0); 
    m_RenderCommandEncoder->setVertexBuffer(m_VertexBuffer->GetVertexBuffer(), 0, 0);
    m_RenderCommandEncoder->drawPrimitives(MTL::PrimitiveTypeTriangleStrip, NS::UInteger(0), NS::UInteger(4));

}

void MetalRenderer::Render(MTL::RenderPassDescriptor* p_RenderPassDescriptor)
{
    m_MetalCommandBuffer = m_MetalCommandQueue->commandBuffer();
    m_RenderPassDescriptor = p_RenderPassDescriptor;
    
    m_RenderCommandEncoder = m_MetalCommandBuffer->renderCommandEncoder(p_RenderPassDescriptor);
    m_RenderCommandEncoder->setFrontFacingWinding(MTL::WindingCounterClockwise);
    m_RenderCommandEncoder->setRenderPipelineState(m_Shader.GetRenderPipelineState());
    m_RenderCommandEncoder->setFragmentTexture(m_Texture->GetTexture(), 0);
    m_RenderCommandEncoder->setVertexBuffer(m_VertexBuffer->GetVertexBuffer(), 0, 0);
    m_RenderCommandEncoder->drawPrimitives(MTL::PrimitiveTypeTriangleStrip, NS::UInteger(0), NS::UInteger(4));

}

void MetalRenderer::Commit(const bool p_Present)
{
    m_RenderCommandEncoder->endEncoding();
    
    if (p_Present && m_MTKView && m_MTKView->currentDrawable())
    {
        m_MetalCommandBuffer->presentDrawable(m_MTKView->currentDrawable());
    }
    m_MetalCommandBuffer->commit();
    m_MetalCommandBuffer->waitUntilCompleted();
}
