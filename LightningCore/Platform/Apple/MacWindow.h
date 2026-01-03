//
// Created by Kian Marvi on 11/24/24.
//

#ifndef MACAPPLICATION_H
#define MACAPPLICATION_H

namespace MTL
{
    class Device;
}

namespace CA
{
    class MetalLayer;
    class MetalDrawable;
}

struct GLFWwindow;
class MetalRenderer;

class MacWindow
{

public:
    explicit MacWindow(unsigned int p_Width = 1280, unsigned int p_Height = 720, const char* p_Title = "");
    bool Update();
    ~MacWindow();


    inline MTL::Device* GetDevice() { return m_MetalDevice; }
    inline CA::MetalLayer* GetMetalLayer() { return m_MetalLayer; }
    inline CA::MetalDrawable* GetMetalDrawable() { return m_MetalDrawable; }
    inline unsigned int GetWidth() { return m_Width; }
    inline unsigned int GetHeight() { return m_Height; }
    inline GLFWwindow* GetWindow() { return m_GlfwWindow; }

private:
    
    static void frameBufferSizeCallback(GLFWwindow *window, int width, int height);
    void resizeFrameBuffer(int width, int height);
    
    unsigned int m_Width, m_Height;
    const char* m_Title;
    
    MTL::Device* m_MetalDevice;
    CA::MetalLayer* m_MetalLayer;
    CA::MetalDrawable* m_MetalDrawable;
    GLFWwindow* m_GlfwWindow;
    MetalRenderer* m_MetalRenderer;
};

#endif //MACAPPLICATION_H
