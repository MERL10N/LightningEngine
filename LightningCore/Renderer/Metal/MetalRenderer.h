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
    class RenderPassColorAttachmentDescriptor;
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
#include "../../Timer/Timer.h"
#include "../../Primitives/MeshBuilder.h"
#include <simd/simd.h>

class MetalVertexBuffer;

class MetalRenderer : public Renderer<MetalRenderer>
{
public:
    MetalRenderer(MTL::Device* p_MetalDevice);
    ~MetalRenderer();

    void CreateTriangle();
    void Render(MTK::View* p_MetalKitView);

private:
    MTL::Device* m_MetalDevice;
    MTL::CommandQueue* m_MetalCommandQueue;
    MTL::CommandBuffer* m_MetalCommandBuffer;
    MTL::RenderPassDescriptor* m_RenderPassDescriptor;
    MTL::RenderCommandEncoder* m_RenderCommandEncoder;
    MTL::RenderPassColorAttachmentDescriptor* m_RenderPassColorAttachmentDescriptor;
    MetalVertexBuffer* m_VertexBuffer;
    MTK::View* metalKitView;
    
    MetalShader m_Shader;

    
    Timer m_Timer;
    
};
#endif //METALRENDERER_H
