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
    
    void Render(MTL::RenderPassDescriptor* p_RenderPassDescriptor);
    
    void Commit();
    
    inline MTL::Device* GetMetalDevice() { return m_MetalDevice; }
    
    inline MTL::CommandBuffer* GetMetalCommandBuffer() { return m_MetalCommandBuffer; }
    
    inline MTL::RenderPassDescriptor* GetMetalRenderPassDescriptor() { return m_RenderPassDescriptor; }
    
    inline void SetRenderCommandEncoder(MTL::RenderCommandEncoder* p_RenderCommandEncoder) {  m_RenderCommandEncoder = p_RenderCommandEncoder; }
    
    inline MTL::RenderCommandEncoder* GetMetalRenderCommandEncoder() { return m_RenderCommandEncoder; }

private:
    MTL::Device* m_MetalDevice;
    MTL::CommandQueue* m_MetalCommandQueue;
    MTL::CommandBuffer* m_MetalCommandBuffer;
    MTL::RenderPassDescriptor* m_RenderPassDescriptor;
    MTL::RenderCommandEncoder* m_RenderCommandEncoder;
    MTL::RenderPipelineState* m_RenderToTexturePipelineState;
    MetalVertexBuffer* m_VertexBuffer;
    MTK::View* m_MTKView;
    
    MetalShader m_Shader;
    
    MetalTexture* m_Texture;

    
    //Timer m_Timer;
    
};
#endif //METALRENDERER_H
