//
//  MetalTexture.cpp
//  LightningCore
//
//  Created by Kian Marvi on 6/15/25.
//

#include "MetalTexture.h"

#include "Metal/Metal.hpp"
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#include <iostream>

MetalTexture::MetalTexture(const char* p_Filepath)
: m_Filepath(p_Filepath),
  m_TextureDescriptor(MTL::TextureDescriptor::alloc()->init())
{
}

void MetalTexture::SetMetalDevice(MTL::Device *p_MetalDevice)
{
    m_MetalDevice = p_MetalDevice;
    stbi_set_flip_vertically_on_load(true);
    unsigned char* image = stbi_load(m_Filepath, &m_Width, &m_Height, &m_Channels, STBI_rgb_alpha);
    assert(image);
    
    if (image)
    {
        std::println("Image found at {} ", m_Filepath);
    }
    
    m_TextureDescriptor->setPixelFormat(MTL::PixelFormatRGBA8Unorm);
    
    m_TextureDescriptor->setWidth(m_Width);
    m_TextureDescriptor->setHeight(m_Height);

    m_Texture = m_MetalDevice->newTexture(m_TextureDescriptor);

    MTL::Region region = MTL::Region(0, 0, 0, m_Width, m_Height, 1);
    NS::UInteger bytesPerRow = 4 * m_Width;

    m_Texture->replaceRegion(region, 0, image, bytesPerRow);

    m_TextureDescriptor->release();
    stbi_image_free(image);
}


MetalTexture::~MetalTexture()
{
    m_Texture->release();
}

