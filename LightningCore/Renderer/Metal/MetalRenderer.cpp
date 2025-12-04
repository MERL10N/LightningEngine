//
// Created by Kian Marvi on 11/24/24.
//

#include "MetalRenderer.h"
#include "Metal/Metal.hpp"
#include "QuartzCore/CAMetalLayer.hpp"
#include "MeshBuilder.h"
#include "MetalTexture.h"
#include "MetalBuffer.h"
#include "../../Primitives/MeshBuilder.h"

MetalRenderer::MetalRenderer(MTL::Device* p_MetalDevice, CA::MetalLayer* p_MetalLayer)
: m_MetalDevice(p_MetalDevice),
  m_MetalLayer(p_MetalLayer),
  m_MetalCommandQueue(m_MetalDevice->newCommandQueue()),
  m_Shader("../../../Shaders/Shader.metal", p_MetalDevice, p_MetalLayer->pixelFormat()),
  m_VertexBuffer(new MetalVertexBuffer(m_MetalDevice)),
  m_RenderPassDescriptor(MTL::RenderPassDescriptor::alloc()->init())
{
    assert(m_MetalDevice);
    CreateQuad("../../../Assets/megaman.png", 1.0f, 1.0f);
   
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
    
    if (m_RenderPassDescriptor)
    {
        m_RenderPassDescriptor->release();
        m_RenderPassDescriptor = nullptr;
    }
}

void MetalRenderer::CreateQuad(const char* p_FilePath, float p_Width, float p_Height)
{
    m_Texture = new MetalTexture(p_FilePath);
    m_Texture->SetMetalDevice(m_MetalDevice);
    MeshBuilder::GenerateQuad(m_VertexBuffer, p_Width, p_Height);
}

void MetalRenderer::BeginFrame()
{
    m_MetalCommandBuffer = m_MetalCommandQueue->commandBuffer();
}

void MetalRenderer::Render()
{
    m_MetalDrawable = m_MetalLayer->nextDrawable();
    m_RenderPassColorAttachmentDescriptor = m_RenderPassDescriptor->colorAttachments()->object(0);
    m_RenderPassColorAttachmentDescriptor->setTexture(m_MetalDrawable->texture());
    m_RenderPassColorAttachmentDescriptor->setLoadAction(MTL::LoadActionClear);
    m_RenderPassColorAttachmentDescriptor->setClearColor(MTL::ClearColor(0.15f, 0.15f, 0.15f, 1.0));
    m_RenderPassColorAttachmentDescriptor->setStoreAction(MTL::StoreActionStore);
 
    m_RenderCommandEncoder = m_MetalCommandBuffer->renderCommandEncoder(m_RenderPassDescriptor);
    m_RenderCommandEncoder->setFrontFacingWinding(MTL::WindingCounterClockwise);
    m_RenderCommandEncoder->setRenderPipelineState(m_Shader.GetRenderPipelineState());
    
    if (m_Texture != nullptr)
    {
        m_RenderCommandEncoder->setFragmentTexture(m_Texture->GetTexture(), 0);
        m_RenderCommandEncoder->setVertexBuffer(m_VertexBuffer->GetVertexBuffer(), 0, 0);
        m_RenderCommandEncoder->drawPrimitives(MTL::PrimitiveTypeTriangleStrip, NS::UInteger(0), NS::UInteger(4));
    }

}

void MetalRenderer::Commit()
{
    m_RenderCommandEncoder->endEncoding();
    m_MetalCommandBuffer->presentDrawable(m_MetalDrawable);
    m_MetalCommandBuffer->commit();
}
