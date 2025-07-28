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

namespace MTK
{
class View;
}

#include <stdint.h>

class MetalFrameBuffer
{
public:
    MetalFrameBuffer(MTK::View* p_MetalKitView);;
    ~MetalFrameBuffer();
    
    void Create(float p_Width, float p_Height);
    void Resize(float p_Width, float p_Height);
    
    void UpdateViewport(MTL::RenderCommandEncoder* p_Encoder);
    
    inline MTL::Texture* GetAttachmentTexture() { return m_AttachmentTexture; }
    inline MTL::RenderPassDescriptor* GetRenderPassDescriptor() { return m_RenderPassDescriptor; }

private:
    MTL::Device* m_MetalDevice;
    MTK::View* m_MetalKitView;
    
    MTL::Texture* m_AttachmentTexture;
    MTL::Texture* m_DepthTexture;
    
    MTL::RenderPassDescriptor* m_RenderPassDescriptor;
    MTL::TextureDescriptor* m_TextureDescriptor;
    MTL::TextureDescriptor* m_DepthTextureDescriptor;
    MTL::RenderPassColorAttachmentDescriptor* m_ColorAttachmentDescriptor;
    MTL::RenderPassDepthAttachmentDescriptor* m_DepthAttachmentDescriptor;
    MTL::RenderCommandEncoder* m_RenderCommandEncoder;
    
    float m_Width, m_Height;
};

#endif /* MetalFrameBuffer_hpp */
