//
//  Scene2D.h
//  LightningEngine
//
//  Created by Kian Marvi on 2/2/24.
//

#ifndef Scene2D_hpp
#define Scene2D_hpp

namespace CA
{
    class MetalLayer;
    class MetalDrawable;
}

namespace MTL
{
    class Device;
    class Library;
    class CommandQueue;
    class CommandBuffer;
    class RenderPipelineState;
    class Buffer;
    class RenderCommandEncoder;
    class RenderPassDescriptor;
}

namespace NS
{
    class AutoreleasePool;
}


class CScene2D
{
public:
    CScene2D(MTL::Device* metalDevice);
    ~CScene2D();
    
    void createSquare();
    void PreRender();
    void Render(CA::MetalDrawable* metalDrawable);
    void PostRender();
private:
    
    MTL::Device* metalDevice;
    
    CA::MetalDrawable* metalDrawable;
    
    NS::AutoreleasePool* pPool;
    
    MTL::Library* metalDefaultLibrary;
    MTL::CommandQueue* metalCommandQueue;
    MTL::CommandBuffer* metalCommandBuffer;
    MTL::RenderPipelineState* metalRenderPSO;
    MTL::Buffer* squareVertexBuffer;
    MTL::Buffer* ArgumentBuffer;
    MTL::RenderCommandEncoder* renderCommandEncoder;
    MTL::RenderPassDescriptor* renderPassDescriptor;
};

#endif /* Scene2D_hpp */
