//
//  MetalFrameBuffer.cpp
//  LightningCore
//
//  Created by Kian Marvi on 7/15/25.
//

#include "MetalFrameBuffer.h"
#include "Metal/Metal.hpp"
#include "MetalShader.h"
#include <iostream>

MetalFrameBuffer::MetalFrameBuffer(MTK::View* p_MetalKitView)
: m_Width(0.f),
  m_Height(0.f),
  m_MetalDevice()
{
}

MetalFrameBuffer::~MetalFrameBuffer()
{
    if (m_AttachmentTexture)
    {
        m_AttachmentTexture->release();
        m_AttachmentTexture = nullptr;
    }
    
    if (m_RenderPassDescriptor)
    {
        m_RenderPassDescriptor->release();
        m_RenderPassDescriptor = nullptr;
    }

    if (m_MetalDevice)
    {
        m_MetalDevice->release();
        m_MetalDevice = nullptr;
    }
    
    if (m_DepthAttachmentDescriptor)
    {
        m_DepthAttachmentDescriptor->release();
        m_DepthAttachmentDescriptor = nullptr;
    }
    
    if (m_ColorAttachmentDescriptor)
    {
        m_ColorAttachmentDescriptor->release();
        m_ColorAttachmentDescriptor = nullptr;
    }
    
    if (m_DepthTexture)
    {
        m_DepthTexture->release();
        m_DepthTexture = nullptr;
    }
    
 
}

void MetalFrameBuffer::Create(float p_Width, float p_Height)
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
    
    
    if (m_DepthTextureDescriptor)
    {
        m_DepthTextureDescriptor->release();
    }
    if (m_TextureDescriptor)
    {
        m_TextureDescriptor->release();
    }

}

void MetalFrameBuffer::UpdateViewport(MTL::RenderCommandEncoder *p_Encoder)
{
    p_Encoder->setViewport(MTL::Viewport{0, 0, (double)m_Width, (double)m_Height});
}

void MetalFrameBuffer::Resize(float p_Width, float p_Height)
{
    if (m_AttachmentTexture)
        m_AttachmentTexture->release();
    
    if (m_DepthTexture)
        m_DepthTexture->release();
    
    if (m_RenderPassDescriptor)
        m_RenderPassDescriptor->release();
    
    if (p_Width <= 1 || p_Height <= 1)
    {
        p_Width = 1;
        p_Height = 1;
    }
    
    
    m_Width = p_Width;
    m_Height = p_Height;
    
    assert(m_Width >= 1);
    assert(m_Height >= 1);

    Create(m_Width, m_Height);
}
