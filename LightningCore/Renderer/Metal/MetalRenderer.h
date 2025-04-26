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
#include "../../Primitives/MeshBuilder.h"
#include "../../Timer/Timer.h"

class MetalRenderer : public Renderer<MetalRenderer>
{
public:
    MetalRenderer(MTL::Device* metalDevice);
    ~MetalRenderer();

    void CreateTriangle();
    void Render(const MTK::View* metalKitView);

private:
    MTL::Device* metalDevice;
    MTL::CommandQueue* metalCommandQueue;
    MTL::CommandBuffer* metalCommandBuffer;
    MTL::RenderPassDescriptor* renderPassDescriptor;
    MTL::RenderCommandEncoder* renderCommandEncoder;
    MTL::RenderPassColorAttachmentDescriptor* renderPassColorAttachmentDescriptor;
    MTL::Buffer* triangleVertexBuffer;
    MTK::View* metalKitView;
    
    MetalShader shader;
    MeshBuilder meshBuilder;
    
    Timer timer;
    
};
#endif //METALRENDERER_H
