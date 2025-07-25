//
//  MetalFrameBuffer.cpp
//  LightningCore
//
//  Created by Kian Marvi on 7/15/25.
//

#include "MetalFrameBuffer.h"
#include <Metal/Metal.hpp>
#include <MetalKit/MetalKit.hpp>
#include "MetalShader.h"
#include <iostream>

MetalFrameBuffer::MetalFrameBuffer(MTK::View* p_MetalKitView)
: m_Width(0),
  m_Height(0),
  m_MetalKitView(p_MetalKitView),
  m_MetalDevice(p_MetalKitView->device())
{
}

MetalFrameBuffer::~MetalFrameBuffer()
{
    if (m_AttachmentTexture)
        m_AttachmentTexture->release();
    
    if (m_RenderPassDescriptor)
        m_RenderPassDescriptor->release();

    if (m_MetalDevice)
        m_MetalDevice->release();
    
    m_DepthAttachmentDescriptor->release();
    m_ColorAttachmentDescriptor->release();
}

void MetalFrameBuffer::Create(uint32_t p_Width, uint32_t p_Height)
{
    m_TextureDescriptor = MTL::TextureDescriptor::alloc()->init();
    m_TextureDescriptor->setWidth(p_Width);
    m_TextureDescriptor->setHeight(p_Height);
    m_TextureDescriptor->setPixelFormat(MTL::PixelFormatRGBA8Unorm);
    m_TextureDescriptor->setTextureType(MTL::TextureType2D);
    m_TextureDescriptor->setStorageMode(MTL::StorageModePrivate);
    m_TextureDescriptor->setUsage(MTL::TextureUsageRenderTarget | MTL::TextureUsageShaderRead);
    m_AttachmentTexture = m_MetalDevice->newTexture(m_TextureDescriptor);
    
    m_DepthTextureDescriptor = MTL::TextureDescriptor::alloc()->init();
    m_DepthTextureDescriptor->setWidth(p_Width);
    m_DepthTextureDescriptor->setHeight(p_Height);
    m_DepthTextureDescriptor->setPixelFormat(MTL::PixelFormatDepth32Float);
    m_DepthTextureDescriptor->setStorageMode(MTL::StorageModePrivate);
    m_DepthTextureDescriptor->setUsage(MTL::TextureUsageRenderTarget);
    m_DepthTexture = m_MetalDevice->newTexture(m_DepthTextureDescriptor);
    
    m_RenderPassDescriptor = MTL::RenderPassDescriptor::alloc()->init();
    m_ColorAttachmentDescriptor = m_RenderPassDescriptor->colorAttachments()->object(0);
    m_ColorAttachmentDescriptor->setTexture(m_AttachmentTexture);
    m_ColorAttachmentDescriptor->setLoadAction(MTL::LoadActionClear);
    m_ColorAttachmentDescriptor->setClearColor(MTL::ClearColor(0.15, 0.15, 0.15, 1.0));
    m_ColorAttachmentDescriptor->setStoreAction(MTL::StoreActionStore);
    
    
    m_DepthAttachmentDescriptor = m_RenderPassDescriptor->depthAttachment();
    m_DepthAttachmentDescriptor->setTexture(m_DepthTexture);
    m_DepthAttachmentDescriptor->setClearDepth(1.0);
    m_DepthAttachmentDescriptor->setStoreAction(MTL::StoreActionDontCare);
    
    
    m_DepthTexture->release();
    m_TextureDescriptor->release();
    m_DepthTextureDescriptor->release();
    
}

void MetalFrameBuffer::UpdateViewport(MTL::RenderCommandEncoder *p_Encoder)
{
    p_Encoder->setViewport(MTL::Viewport{0, 0, (double)m_Width, (double)m_Height});
    std::println("m_Width {}, m_Height {}", m_Width, m_Height);
}

void MetalFrameBuffer::Resize(uint32_t p_Width, uint32_t p_Height)
{
    if (m_AttachmentTexture)
        m_AttachmentTexture->release();
    
    if (m_RenderPassDescriptor)
        m_RenderPassDescriptor->release();
    
    if (p_Width <= 0 || p_Height <= 0)
        return;
    
    
    m_Width = p_Width;
    m_Height = p_Height;

    Create(m_Width, m_Height);
}
