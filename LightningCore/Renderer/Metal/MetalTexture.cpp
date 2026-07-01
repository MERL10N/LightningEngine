//
//  MetalTexture.cpp
//  LightningCore
//
//  Created by Kian Marvi on 6/15/25.

#include "MetalTexture.h"

#include "Metal/Metal.hpp"
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#include <iostream>

MetalTexture::MetalTexture(const char* p_FilePath, MTL::Device* p_MetalDevice)
: m_Filepath(p_FilePath),
  m_MetalDevice(p_MetalDevice),
  m_TextureDescriptor(MTL::TextureDescriptor::alloc()->init())
{
    assert(m_MetalDevice);
    stbi_set_flip_vertically_on_load(true);
    unsigned char* image = stbi_load(m_Filepath, &m_Width, &m_Height, &m_Channels, STBI_rgb_alpha);
    assert(image);
    
    if (image)
    {
        std::println("Image found at: {} ", m_Filepath);
    }
    
    
    m_TextureDescriptor->setPixelFormat(MTL::PixelFormatRGBA8Unorm);
    
    m_TextureDescriptor->setWidth(m_Width);
    m_TextureDescriptor->setHeight(m_Height);
    m_TextureDescriptor->setUsage(MTL::TextureUsageShaderRead | MTL::TextureUsageShaderWrite);

    m_Texture = m_MetalDevice->newTexture(m_TextureDescriptor);

    MTL::Region region = MTL::Region(0, 0, 0, m_Width, m_Height, 1);
    NS::UInteger bytesPerRow = 4 * m_Width;

    m_Texture->replaceRegion(region, 0, image, bytesPerRow);
    
    //InitialiseTextureArguments();
    
    
    if (m_MetalDevice->argumentBuffersSupport() == MTL::ArgumentBuffersTier2)
    {
        m_ArgumentBuffer = m_MetalDevice->newBuffer(sizeof(MTL::ResourceID), MTL::ResourceStorageModeManaged);
        MTL::ResourceID textureID = m_Texture->gpuResourceID();
        memcpy(m_ArgumentBuffer->contents(), &textureID, sizeof(MTL::ResourceID));
        m_ArgumentBuffer->didModifyRange(NS::Range(0, sizeof(MTL::ResourceID)));
    }

    m_TextureDescriptor->release();
    stbi_image_free(image);
}


MetalTexture::~MetalTexture()
{
    if (m_ArgumentBuffer)
    {
        m_ArgumentBuffer->release();
        m_ArgumentBuffer = nullptr;
    }
    
    if (m_MetalDevice)
    {
        m_MetalDevice->release();
        m_MetalDevice = nullptr;
    }
    
    if (m_Texture)
    {
        std::println("Delete texture at: {}", m_Filepath);
        m_Texture->release();
        m_Texture = nullptr;
    }
}

void MetalTexture::InitialiseTextureArguments()
{
   
}


