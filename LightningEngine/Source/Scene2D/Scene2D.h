//
//  Scene2D.h
//  LightningEngine
//
//  Created by Kian Marvi on 2/2/24.
//

#ifndef Scene2D_hpp
#define Scene2D_hpp

#include <Metal/Metal.hpp>
#include <QuartzCore/CAMetalLayer.hpp>
#include <QuartzCore/QuartzCore.hpp>

class CScene2D
{
public:
    CScene2D();
    ~CScene2D();
    
    void createSquare();
    void PreRender();
    void Render();
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
