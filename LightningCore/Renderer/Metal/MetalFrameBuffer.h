//
//  MetalFrameBuffer.hpp
//  LightningCore
//
//  Created by Kian Marvi on 7/15/25.
//

#ifndef MetalFrameBuffer_hpp
#define MetalFrameBuffer_hpp

namespace MTL
{
    class Device;
    class Texture;
    class TextureDescriptor;
    class RenderPassDescriptor;
    class RenderPassColorAttachmentDescriptor;
    class RenderPassDepthAttachmentDescriptor;
    class RenderCommandEncoder;
    class Library;
}

#include <stdint.h>

class MetalFrameBuffer
{
public:
    MetalFrameBuffer(MTL::Device* p_MetalDevice);;
    ~MetalFrameBuffer();
    
    void Create(float p_Width, float p_Height);
    void Resize(float p_Width, float p_Height);
    
    void UpdateViewport(MTL::RenderCommandEncoder* p_Encoder);
    
    inline MTL::Texture* GetAttachmentTexture() { return m_AttachmentTexture; }
    inline MTL::RenderPassDescriptor* GetRenderPassDescriptor() { return m_RenderPassDescriptor; }

private:
    MTL::Device* m_MetalDevice = nullptr;
    MTL::Texture* m_AttachmentTexture = nullptr;
    MTL::Texture* m_DepthTexture = nullptr;
    
    MTL::RenderPassDescriptor* m_RenderPassDescriptor = nullptr;
    MTL::TextureDescriptor* m_TextureDescriptor = nullptr;
    MTL::TextureDescriptor* m_DepthTextureDescriptor = nullptr;
    MTL::RenderPassColorAttachmentDescriptor* m_ColorAttachmentDescriptor = nullptr;
    MTL::RenderPassDepthAttachmentDescriptor* m_DepthAttachmentDescriptor = nullptr;
    
    float m_Width, m_Height;
};

#endif /* MetalFrameBuffer_hpp */
