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
    int width, height, channels;
    unsigned char* image = stbi_load(m_Filepath, &width, &height, &channels, STBI_rgb_alpha);
    assert(image);
    
    if (image)
    {
        std::println("Image found at: {} ", m_Filepath);
    }
    
    
    m_TextureDescriptor->setPixelFormat(MTL::PixelFormatRGBA8Unorm);
    
    m_TextureDescriptor->setWidth(width);
    m_TextureDescriptor->setHeight(height);
    m_TextureDescriptor->setUsage(MTL::TextureUsageShaderRead | MTL::TextureUsageShaderWrite);

    m_Texture = m_MetalDevice->newTexture(m_TextureDescriptor);
    m_TextureDescriptor->release();

    MTL::Region region = MTL::Region(0, 0, 0, width, height, 1);
    NS::UInteger bytesPerRow = 4 * width;

    m_Texture->replaceRegion(region, 0, image, bytesPerRow);
    
    
    if (m_MetalDevice->argumentBuffersSupport() == MTL::ArgumentBuffersTier2)
    {
        m_ArgumentBuffer = m_MetalDevice->newBuffer(sizeof(MTL::ResourceID), MTL::ResourceStorageModeShared);
        MTL::ResourceID textureID = m_Texture->gpuResourceID();
        memcpy(m_ArgumentBuffer->contents(), &textureID, sizeof(MTL::ResourceID));
    }
    stbi_image_free(image);
}

MetalTexture::MetalTexture(const std::vector<const char*> &filePaths, MTL::Device *metalDevice)
: m_MetalDevice(metalDevice),
  m_TextureDescriptor(MTL::TextureDescriptor::alloc()->init())
{

    std::vector<unsigned char*> images;
    images.reserve(2);
    m_Widths.reserve(2);
    m_Heights.reserve(2);
    m_TextureInfos.reserve(2);
    
    int maxImageWidth = 0;
    int maxImageHeight = 0;
    int width, height, channels;
    
    
    for (const auto& filePath : filePaths)
    {
        stbi_set_flip_vertically_on_load(true);
        unsigned char* image = stbi_load(filePath, &width, &height, &channels, STBI_rgb_alpha);
        
        
        if (image)
        {
            std::println("Image found at: {} ", filePath);
            maxImageWidth = std::max(maxImageWidth, width);
            maxImageHeight = std::max(maxImageHeight, height);
            
            m_Widths.emplace_back(width);
            m_Heights.emplace_back(height);
            images.emplace_back(image);
        }
    }
    
    m_TextureDescriptor->texture2DDescriptor(MTL::PixelFormatRGBA8Unorm, maxImageWidth, maxImageHeight, false);
    
    m_TextureDescriptor->setArrayLength(images.size());
    m_TextureDescriptor->setUsage(MTL::TextureUsageShaderRead | MTL::TextureUsageShaderWrite);
    m_TextureDescriptor->setTextureType(MTL::TextureType2DArray);
    m_TextureDescriptor->setWidth(maxImageWidth);
    m_TextureDescriptor->setHeight(maxImageHeight);
    m_TextureDescriptor->setPixelFormat(MTL::PixelFormatRGBA8Unorm);
    m_Texture = m_MetalDevice->newTexture(m_TextureDescriptor);
    m_TextureDescriptor->release();
    
    if (m_MetalDevice->argumentBuffersSupport() == MTL::ArgumentBuffersTier2)
    {
        m_ArgumentBuffer = m_MetalDevice->newBuffer(sizeof(MTL::ResourceID), MTL::ResourceStorageModeShared);
        MTL::ResourceID textureID = m_Texture->gpuResourceID();
        memcpy(m_ArgumentBuffer->contents(), &textureID, sizeof(MTL::ResourceID));
    }

    for (int i = 0; i < images.size(); ++i)
    {
        m_TextureInfos.emplace_back(m_Widths[i], m_Heights[i]);
        
        MTL::Region region = MTL::Region(0, 0, 0, m_Widths[i], m_Heights[i], 1);
        NS::UInteger bytesPerRow = 4 * m_Widths[i];
                
        m_Texture->replaceRegion(region, 0, i, images[i], bytesPerRow, 0);
        
        stbi_image_free(images[i]);
    }
}




MetalTexture::~MetalTexture()
{
    if (m_ArgumentBuffer)
    {
        m_ArgumentBuffer->release();
        m_ArgumentBuffer = nullptr;
    }
    
    if (m_Texture)
    {
        if (m_Filepath)
        {
            std::println("Delete texture at: {}", m_Filepath);
        }
        m_Texture->release();
        m_Texture = nullptr;
    }
    
    if (m_MetalDevice)
    {
        m_MetalDevice->release();
        m_MetalDevice = nullptr;
    }
}
