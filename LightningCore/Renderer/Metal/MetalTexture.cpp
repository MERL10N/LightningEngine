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
  m_MetalDevice(p_MetalDevice)
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
    
    MTL::TextureDescriptor* m_TextureDescriptor = MTL::TextureDescriptor::alloc()->init();
    m_TextureDescriptor->setPixelFormat(MTL::PixelFormatRGBA8Unorm);
    m_TextureDescriptor->setWidth(width);
    m_TextureDescriptor->setHeight(height);
    m_TextureDescriptor->setUsage(MTL::TextureUsageShaderRead);
    
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
: m_MetalDevice(metalDevice)
{
    std::vector<MTL::ResourceID> gpuResourceIDs(4, MTL::ResourceID{0});
    m_TextureInfos.reserve(4);
    m_Textures.reserve(4);
    int width, height, channels;
    
    
    for (size_t i = 0; i < filePaths.size(); ++i)
    {
        if (filePaths[i] != nullptr)
        {
            stbi_set_flip_vertically_on_load(true);
            unsigned char* image = stbi_load(filePaths[i], &width, &height, &channels, STBI_rgb_alpha);
            
            if (image)
            {
                const uint32_t mipLevels = std::floor(std::log2(std::max(width, height))) + 1;
                
                std::println("Image found at {}", filePaths[i]);
                MTL::TextureDescriptor* m_TextureDescriptor = MTL::TextureDescriptor::alloc()->init();
                m_TextureDescriptor->setTextureType(MTL::TextureType2D);
                m_TextureDescriptor->setMipmapLevelCount(mipLevels);
                m_TextureDescriptor->setUsage(MTL::TextureUsageShaderRead | MTL::TextureUsageRenderTarget);
                m_TextureDescriptor->setWidth(width);
                m_TextureDescriptor->setHeight(height);
                m_TextureDescriptor->setPixelFormat(MTL::PixelFormatRGBA8Unorm);
                MTL::Texture* textureElement = m_MetalDevice->newTexture(m_TextureDescriptor);
                m_TextureDescriptor->release();
                
                MTL::Region region = MTL::Region(0, 0, 0, width, height, 1);
                NS::UInteger bytesPerRow = 4 * width;
                textureElement->replaceRegion(region, 0, 0, image, bytesPerRow, 0);
                stbi_image_free(image);
                m_Textures.emplace_back(textureElement);
                gpuResourceIDs[i] = textureElement->gpuResourceID();
                continue;
            }
        }
    }
    
    GenerateMipmaps();
    
    if (m_MetalDevice->argumentBuffersSupport() == MTL::ArgumentBuffersTier2)
    {
        m_ArgumentBuffer = m_MetalDevice->newBuffer(4 * sizeof(MTL::ResourceID), MTL::ResourceStorageModeShared);
        memcpy(m_ArgumentBuffer->contents(), gpuResourceIDs.data(), 4 * sizeof(MTL::ResourceID));
    }

}

MetalTexture::MetalTexture(const std::array<const char*, 6> &faces, MTL::Device* metalDevice)
: m_MetalDevice(metalDevice)
{
    LoadCubeMap(faces);
}

MetalTexture &MetalTexture::operator=(MetalTexture &&other)
{
    if (this == &other)
        return (*this);
    
    for (auto &texture : m_Textures)
    {
        texture->release();
        texture = nullptr;
    }
    m_ArgumentBuffer->release();
    
    m_Filepath = other.m_Filepath;
    m_Textures = other.m_Textures;
    m_ArgumentBuffer = other.m_ArgumentBuffer;
    
    for (auto &otherTexture : other.m_Textures)
    {
        otherTexture->release();
        otherTexture = nullptr;
    }
    
    other.m_ArgumentBuffer->release();
    other.m_ArgumentBuffer = nullptr;
    
    return (*this);
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
        m_Texture->release();
        m_Texture = nullptr;
    }
    
    if (m_CubeMap)
    {
        m_CubeMap->release();
        m_CubeMap = nullptr;
    }
    
    if (m_Textures.size() > 0)
    {
        for (auto& texture : m_Textures)
        {
            if (texture)
            {
                texture->release();
                texture = nullptr;
            }
        }
        m_Textures.clear();
    }
    
    if (m_MetalDevice)
    {
        m_MetalDevice->release();
        m_MetalDevice = nullptr;
    }
}

void MetalTexture::LoadCubeMap(const std::array<const char*, 6> &faces)
{
    stbi_set_flip_vertically_on_load(false);
    int width, height, channels;
    unsigned char* firstImage = stbi_load(faces[0], &width, &height, &channels, STBI_rgb_alpha);
    assert(firstImage);
    

    MTL::TextureDescriptor* cubeMapDescriptor = MTL::TextureDescriptor::textureCubeDescriptor(MTL::PixelFormatRGBA8Unorm, width, false);
    cubeMapDescriptor->setUsage(MTL::TextureUsageShaderRead);
    m_CubeMap = m_MetalDevice->newTexture(cubeMapDescriptor);
    cubeMapDescriptor->release();
    
    const MTL::Region region(0,0,0,width, height, 1);
    NS::UInteger bytesPerRow = 4 * width;
    
    for (size_t i = 0; i < faces.size(); ++i)
    {
        unsigned char* image = (i == 0) ? firstImage : stbi_load(faces[i], &width, &height, &channels, STBI_rgb_alpha);
        m_CubeMap->replaceRegion(region, 0, i, image, bytesPerRow, bytesPerRow * height);
        stbi_image_free(image);
    }
    
    if (m_MetalDevice->argumentBuffersSupport() == MTL::ArgumentBuffersTier2)
    {
        m_ArgumentBuffer = m_MetalDevice->newBuffer(4 * sizeof(MTL::ResourceID), MTL::ResourceStorageModeShared);
        MTL::ResourceID id = m_CubeMap->gpuResourceID();
        memcpy(m_ArgumentBuffer->contents(), &id, sizeof(id));
    }
}

void MetalTexture::GenerateMipmaps()
{
    if (m_Textures.empty())
        return;
    
    MTL::ResidencySetDescriptor* residencySetDescriptor = MTL::ResidencySetDescriptor::alloc()->init();
    MTL::ResidencySet*           residencySet           = m_MetalDevice->newResidencySet(residencySetDescriptor, nullptr);
    residencySetDescriptor->release();
    
    for (auto* texture: m_Textures)
        residencySet->addAllocation(texture);
    
    residencySet->commit();
    residencySet->requestResidency();
    
    MTL4::CommandAllocator* commandAllocator = m_MetalDevice->newCommandAllocator();
    MTL4::CommandBuffer*    commandBuffer    = m_MetalDevice->newCommandBuffer();
    MTL4::CommandQueue*     commandQueue     = m_MetalDevice->newMTL4CommandQueue();
    MTL::SharedEvent*       signalEvent      = m_MetalDevice->newSharedEvent();
    uint64_t signalValue = 0;
    
    commandBuffer->beginCommandBuffer(commandAllocator);
    
    MTL4::ComputeCommandEncoder* computeCommandEncoder =  commandBuffer->computeCommandEncoder();
    
    for (auto* texture: m_Textures)
    {
        if (texture->mipmapLevelCount() > 1)
        {
            computeCommandEncoder->generateMipmaps(texture);
        }
    }
    computeCommandEncoder->endEncoding();
    commandBuffer->endCommandBuffer();
    
    commandQueue->commit(&commandBuffer, 1);
    commandQueue->signalEvent(signalEvent, ++signalValue);
    signalEvent->waitUntilSignaledValue(signalValue, 5000);
    
    
    signalEvent->release();
    commandBuffer->release();
    commandAllocator->release();
    computeCommandEncoder->release();
}


