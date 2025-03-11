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
}

namespace MTK
{
    class View;
}

#include "../Renderer.h"

class MetalRenderer : public Renderer<MetalRenderer>
{
public:
    MetalRenderer(MTL::Device* metalDevice);
    ~MetalRenderer();
    void Initialise(MTL::Device* metalDevice);
    void Draw(const MTK::View* metalKitView);
    void Destroy();

private:
    MTL::Device* metalDevice;
    MTL::CommandQueue* metalCommandQueue;
    MTL::CommandBuffer* metalCommandBuffer;
    MTL::RenderPassDescriptor* renderPassDescriptor;
    MTL::RenderCommandEncoder* renderCommandEncoder;
    MTL::RenderPassColorAttachmentDescriptor* renderPassColorAttachmentDescriptor;
    
    MTK::View* metalKitView;
};



#endif //METALRENDERER_H
