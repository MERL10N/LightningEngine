//
//  MetalFrameBuffer.hpp
//  LightningEngineCore
//
//  Created by Kian Marvi on 9/3/24.
//

#ifndef MetalFrameBuffer_hpp
#define MetalFrameBuffer_hpp

namespace MTL
{
    class Device;
    class Library;
    class CommandQueue;
    class CommandBuffer;
    class RenderPipelineState;
    class Buffer;
    class Texture;
    class ArgumentEncoder;
    class RenderCommandEncoder;
    class RenderPassDescriptor;
    class RenderPassColorAttachmentDescriptor;
    class RenderPassDepthAttachmentDescriptor;
    class TextureDescriptor;
}

struct CGSize;

class MetalFrameBuffer
{
public:
    void Init(float &width, float &height, MTL::Device* device, const int &sampleCount);
    void Release();
    void ResizeFrameBuffer(float &width, float &height, CGSize &size, MTL::Device* device, const int &sampleCount);
    MTL::Texture*  GetTargetTexture();
    MTL::Texture*  GetDepthTexture();
    MTL::Texture*  GetResolvedTexture();
    
private:
    MTL::Texture* msaaRenderTargetTexture = nullptr;
    MTL::Texture* depthTexture = nullptr;
    MTL::Texture* resolvedTexture = nullptr;
    MTL::TextureDescriptor* msaaTextureDescriptor = nullptr;
    MTL::TextureDescriptor* depthTextureDescriptor = nullptr;
    MTL::TextureDescriptor* resolvedTextureDescriptor = nullptr;
    float width;
    float height;
};

#endif /* MetalFrameBuffer_hpp */
