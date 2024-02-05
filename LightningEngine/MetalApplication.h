//
//  mtl_engine.hpp
//  MetalTutorial

#ifndef MetalApplication_h
#define MetalApplication_h

#include <DesignPatterns/SingletonTemplate.h>

#include <iostream>
#include <filesystem>

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

struct GLFWwindow;

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
