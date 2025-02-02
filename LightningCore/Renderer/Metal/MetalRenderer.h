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
    void InitMetal(MTL::Device* metalDevice);
    void RenderMetal(const MTK::View* metalKitView);
    void CleanMetal();

private:
    MTL::Device* metalDevice;
    MTL::CommandQueue* metalCommandQueue;
    MTL::CommandBuffer* metalCommandBuffer;
    MTL::RenderPassDescriptor* renderPassDescriptor;
    MTL::RenderCommandEncoder* renderCommandEncoder;
    MTK::View* metalKitView;
};



#endif //METALRENDERER_H
