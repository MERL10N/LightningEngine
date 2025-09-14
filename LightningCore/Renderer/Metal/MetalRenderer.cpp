//
// Created by Kian Marvi on 11/24/24.
//

#include "MetalRenderer.h"
#include <Metal/Metal.hpp>
#include <MetalKit/MetalKit.hpp>
#include "MeshBuilder.h"
#include "../../Primitives/SpriteAnimation.h"
#include "MetalTexture.h"
#include "MetalBuffer.h"
#include "../../Primitives/MeshBuilder.h"

MetalRenderer::MetalRenderer(MTK::View* p_MTKView, MTL::PixelFormat p_DepthAttachmentPixelFormat)
: m_MetalDevice(p_MTKView->device()),
  m_MTKView(p_MTKView),
  m_MetalCommandQueue(m_MetalDevice->newCommandQueue()),
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

void MetalRenderer::CreateQuad(const char* p_FilePath)
{
    m_Texture = new MetalTexture(p_FilePath);
    m_Texture->SetMetalDevice(m_MetalDevice);
    MeshBuilder::GenerateQuad(m_VertexBuffer);
}

void MetalRenderer::BeginFrame()
{
    m_MetalCommandBuffer = m_MetalCommandQueue->commandBuffer();
}

void MetalRenderer::Render(MTL::RenderPassDescriptor* p_RenderPassDescriptor)
{
    m_RenderPassDescriptor = p_RenderPassDescriptor;
    m_RenderCommandEncoder = m_MetalCommandBuffer->renderCommandEncoder(m_RenderPassDescriptor);
    m_RenderCommandEncoder->setFrontFacingWinding(MTL::WindingCounterClockwise);
    m_RenderCommandEncoder->setRenderPipelineState(m_Shader.GetRenderPipelineState());
    m_RenderCommandEncoder->setFragmentTexture(m_Texture->GetTexture(), 0);
    m_RenderCommandEncoder->setVertexBuffer(m_VertexBuffer->GetVertexBuffer(), 0, 0);
    m_RenderCommandEncoder->drawPrimitives(MTL::PrimitiveTypeTriangleStrip, NS::UInteger(0), NS::UInteger(4));

}

void MetalRenderer::Commit()
{
    m_RenderCommandEncoder->endEncoding();
    
    if (m_MTKView)
    {
        m_MetalCommandBuffer->presentDrawable(m_MTKView->currentDrawable());
    }
    m_MetalCommandBuffer->commit();
    m_MetalCommandBuffer->waitUntilCompleted();
}
