//
// Created by Kian Marvi on 11/24/24.
//

#ifndef METALRENDERER_H
#define METALRENDERER_H

namespace MTL
{
    class Device;
    class CommandQueue;
    class CommandBuffer;
    class RenderPassDescriptor;
    class RenderCommandEncoder;
    class Buffer;
}

namespace MTK
{
    class View;
}

namespace CA
{
    class MetalDrawable;
}

class SpriteAnimation;


#include "../Renderer.h"
#include "MetalShader.h"
#include "../../Primitives/MeshBuilder.h"
#include <simd/simd.h>

class MetalVertexBuffer;
class MetalTexture;

class MetalRenderer : public Renderer<MetalRenderer>
{
public:
    MetalRenderer(MTK::View* p_MTKView, MTL::PixelFormat p_DepthAttachmentPixelFormat = MTL::PixelFormatInvalid);
    ~MetalRenderer();
    
    void BeginFrame();

    void CreateQuad(const char* p_TextureFilePath);
    
    void AddSprite(const SpriteAnimation &m_Sprite);
    
    void RemoveSprite(const SpriteAnimation &m_Sprite);
    
    void Render(MTL::RenderPassDescriptor* p_RenderPassDescriptor);
    
    void Commit();
    
    inline MTL::Device* GetMetalDevice() { return m_MetalDevice; }
    
    inline MTL::CommandBuffer* GetMetalCommandBuffer() { return m_MetalCommandBuffer; }
    
    inline MTL::RenderPassDescriptor* GetMetalRenderPassDescriptor() { return m_RenderPassDescriptor; }
    
    inline void SetRenderCommandEncoder(MTL::RenderCommandEncoder* p_RenderCommandEncoder) {  m_RenderCommandEncoder = p_RenderCommandEncoder; }
    
    inline MTL::RenderCommandEncoder* GetMetalRenderCommandEncoder() { return m_RenderCommandEncoder; }

private:
    MTL::Device* m_MetalDevice = nullptr;
    MTL::CommandQueue* m_MetalCommandQueue = nullptr;
    MTL::CommandBuffer* m_MetalCommandBuffer = nullptr;
    MTL::RenderPassDescriptor* m_RenderPassDescriptor = nullptr;
    MTL::RenderCommandEncoder* m_RenderCommandEncoder = nullptr;
    MTL::RenderPipelineState* m_RenderToTexturePipelineState = nullptr;
    
    MTK::View* m_MTKView = nullptr;
    
    MetalVertexBuffer* m_VertexBuffer = nullptr;
    
    MetalShader m_Shader;
    
    MetalTexture* m_Texture;

   // std::vector<SpriteAnimation&> m_Queue;
    
    
};
#endif //METALRENDERER_H
