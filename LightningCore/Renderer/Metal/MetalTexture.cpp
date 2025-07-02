//
//  MetalTexture.cpp
//  LightningCore
//
//  Created by Kian Marvi on 6/15/25.
//

#include "MetalTexture.h"

#include <Metal/Metal.hpp>
#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>
#include <iostream>

MetalTexture::MetalTexture(const char* p_Filepath, MTL::Device* p_MetalDevice)
: m_Filepath(p_Filepath),
m_MetalDevice(p_MetalDevice),
  m_TextureDescriptor(MTL::TextureDescriptor::alloc()->init())
{
   
    stbi_set_flip_vertically_on_load(true);
    unsigned char* image = stbi_load(p_Filepath, &width, &height, &channels, STBI_rgb_alpha);
    assert(image != nullptr);
    
    if (image)
    {
        std::cout << "Image found at " << p_Filepath << std::endl;
    }
    
    m_TextureDescriptor->setPixelFormat(MTL::PixelFormatRGBA8Unorm);
    
    m_TextureDescriptor->setWidth(width);
    m_TextureDescriptor->setHeight(height);

    m_Texture = m_MetalDevice->newTexture(m_TextureDescriptor);

    MTL::Region region = MTL::Region(0, 0, 0, width, height, 1);
    NS::UInteger bytesPerRow = 4 * width;

    m_Texture->replaceRegion(region, 0, image, bytesPerRow);

    m_TextureDescriptor->release();
    stbi_image_free(image);
}


MetalTexture::~MetalTexture()
{
    m_Texture->release();
}


MTL::Texture* MetalTexture::GetTexture()
{
    return m_Texture;
}
