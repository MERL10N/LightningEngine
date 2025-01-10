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
}

namespace MTK
{
    class View;
}

class MetalRenderer
{
public:
    explicit MetalRenderer(MTL::Device* metalDevice);
    void Draw(const MTK::View* metalKitView);
    void Clean();

private:
    MTL::Device* metalDevice;
    MTL::CommandQueue* metalCommandQueue;
    MTL::CommandBuffer* metalCommandBuffer;
    MTL::RenderPassDescriptor* renderPassDescriptor;
    MTL::RenderCommandEncoder* renderCommandEncoder;
};



#endif //METALRENDERER_H
