//
//  mtl_engine.hpp
//  MetalTutorial

#ifndef MetalApplication_h
#define MetalApplication_h


#include <Metal/Metal.hpp>
#include <QuartzCore/CAMetalLayer.hpp>
#include <QuartzCore/QuartzCore.hpp>

#include <DesignPatterns/SingletonTemplate.h>

#include <iostream>
#include <filesystem>

struct GLFWwindow;
@class NSWindow;
@class CAMetalLayer;



class CMetalApplication : public CSingletonTemplate<CMetalApplication>
{
    friend CSingletonTemplate<CMetalApplication>;
public:
    bool init();
    void Run();
    void Destroy();

private:
    
    void createSquare();
    
    void draw();
    
    static void frameBufferSizeCallback(GLFWwindow *window, int width, int height);
    void resizeFrameBuffer(int width, int height);
    
    MTL::Device* metalDevice;
    
    GLFWwindow* glfwWindow;
    NSWindow* metalWindow;
    CAMetalLayer* metalLayer;

    
    CA::MetalLayer* layer;
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
#endif
