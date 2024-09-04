//
//  MetalFrameBuffer.cpp
//  LightningEngineCore
//
//  Created by Kian Marvi on 9/3/24.
//

#include "MetalFrameBuffer.h"
#include <CoreGraphics/CoreGraphics.h>
#include <Metal/Metal.hpp>

void MetalFrameBuffer::InitialiseFrameBuffer(float &width, float &height, MTL::Device *device, const int &sampleCount)
{
    msaaTextureDescriptor = MTL::TextureDescriptor::alloc()->init();
    msaaTextureDescriptor->setTextureType(MTL::TextureType2DMultisample);
    msaaTextureDescriptor->setPixelFormat(MTL::PixelFormatRGBA8Unorm);
    msaaTextureDescriptor->setWidth(width);
    msaaTextureDescriptor->setHeight(height);
    msaaTextureDescriptor->setSampleCount(sampleCount);
    msaaTextureDescriptor->setStorageMode(MTL::StorageModePrivate);
    msaaTextureDescriptor->setUsage(MTL::TextureUsageRenderTarget);
    
    msaaRenderTargetTexture = device->newTexture(msaaTextureDescriptor);
    msaaTextureDescriptor->release();

    depthTextureDescriptor = MTL::TextureDescriptor::alloc()->init();
    depthTextureDescriptor->setTextureType(MTL::TextureType2DMultisample);
    depthTextureDescriptor->setPixelFormat(MTL::PixelFormatDepth32Float);
    depthTextureDescriptor->setWidth(width);
    depthTextureDescriptor->setHeight(height);
    depthTextureDescriptor->setSampleCount(sampleCount);
    depthTextureDescriptor->setStorageMode(MTL::StorageModePrivate);
    depthTextureDescriptor->setUsage(MTL::TextureUsageRenderTarget);

    depthTexture = device->newTexture(depthTextureDescriptor);
    depthTextureDescriptor->release();
    
    resolvedTextureDescriptor = MTL::TextureDescriptor::alloc()->init();
    resolvedTextureDescriptor->setTextureType(MTL::TextureType2D);
    resolvedTextureDescriptor->setPixelFormat(MTL::PixelFormatRGBA8Unorm);
    resolvedTextureDescriptor->setWidth(width);
    resolvedTextureDescriptor->setHeight(height);
    resolvedTextureDescriptor->setStorageMode(MTL::StorageModeManaged);
    resolvedTextureDescriptor->setUsage(MTL::TextureUsageShaderRead | MTL::TextureUsageRenderTarget);

    resolvedTexture = device->newTexture(resolvedTextureDescriptor);
    resolvedTextureDescriptor->release();
}

void MetalFrameBuffer::DeallocateFrameBuffer()
{
    if (msaaRenderTargetTexture)
        msaaRenderTargetTexture->release();
    if (depthTexture)
        depthTexture->release();
    if (resolvedTexture)
        resolvedTexture->release();
}


MTL::Texture* MetalFrameBuffer::GetTargetTexture()
{
    return msaaRenderTargetTexture;
}

MTL::Texture* MetalFrameBuffer::GetDepthTexture()
{
    return depthTexture;
}

MTL::Texture* MetalFrameBuffer::GetResolvedTexture()
{
    return resolvedTexture;
}

void MetalFrameBuffer::ResizeFrameBuffer(float &width, float &height, CGSize &size, MTL::Device *device, const int &sampleCount)
{
    width = size.width;;
    height = size.height;
    
    if (msaaRenderTargetTexture)
        msaaRenderTargetTexture->release();
    if (depthTexture)
        depthTexture->release();
    if (resolvedTexture)
        resolvedTexture->release();
    
    msaaTextureDescriptor = MTL::TextureDescriptor::alloc()->init();
    msaaTextureDescriptor->setTextureType(MTL::TextureType2DMultisample);
    msaaTextureDescriptor->setPixelFormat(MTL::PixelFormatRGBA8Unorm);
    msaaTextureDescriptor->setWidth(width);
    msaaTextureDescriptor->setHeight(height);
    msaaTextureDescriptor->setSampleCount(sampleCount);
    msaaTextureDescriptor->setStorageMode(MTL::StorageModePrivate);
    msaaTextureDescriptor->setUsage(MTL::TextureUsageRenderTarget);
    
    msaaRenderTargetTexture = device->newTexture(msaaTextureDescriptor);
    msaaTextureDescriptor->release();

    depthTextureDescriptor = MTL::TextureDescriptor::alloc()->init();
    depthTextureDescriptor->setTextureType(MTL::TextureType2DMultisample);
    depthTextureDescriptor->setPixelFormat(MTL::PixelFormatDepth32Float);
    depthTextureDescriptor->setWidth(width);
    depthTextureDescriptor->setHeight(height);
    depthTextureDescriptor->setSampleCount(sampleCount);
    depthTextureDescriptor->setStorageMode(MTL::StorageModePrivate);
    depthTextureDescriptor->setUsage(MTL::TextureUsageRenderTarget);

    depthTexture = device->newTexture(depthTextureDescriptor);
    depthTextureDescriptor->release();
    
    resolvedTextureDescriptor = MTL::TextureDescriptor::alloc()->init();
    resolvedTextureDescriptor->setTextureType(MTL::TextureType2D);
    resolvedTextureDescriptor->setPixelFormat(MTL::PixelFormatRGBA8Unorm);
    resolvedTextureDescriptor->setWidth(width);
    resolvedTextureDescriptor->setHeight(height);
    resolvedTextureDescriptor->setStorageMode(MTL::StorageModeManaged);
    resolvedTextureDescriptor->setUsage(MTL::TextureUsageShaderRead | MTL::TextureUsageRenderTarget);

    resolvedTexture = device->newTexture(resolvedTextureDescriptor);
    resolvedTextureDescriptor->release();
}
