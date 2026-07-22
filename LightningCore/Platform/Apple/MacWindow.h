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

class MacWindow
{

public:
    explicit MacWindow(const unsigned int p_Width = 1280, const unsigned int p_Height = 720, const char* p_Title = "");
    bool Update();
    ~MacWindow();


    inline MTL::Device* GetDevice() const { return m_MetalDevice; }
    inline CA::MetalLayer* GetMetalLayer() const { return m_MetalLayer; }
    inline CA::MetalDrawable* GetMetalDrawable() const { return m_MetalDrawable; }
    inline unsigned int GetWidth() const { return m_Width; }
    inline unsigned int GetHeight() const { return m_Height; }
    inline GLFWwindow* GetWindow() const { return m_GlfwWindow; }

private:
    
    static void frameBufferSizeCallback(GLFWwindow *window, const int width, const int height);
    void resizeFrameBuffer(const int width, const int height);
    
    unsigned int m_Width, m_Height;
    const char* m_Title;
    
    MTL::Device* m_MetalDevice;
    CA::MetalLayer* m_MetalLayer;
    CA::MetalDrawable* m_MetalDrawable;
    GLFWwindow* m_GlfwWindow;
};

#endif //MACAPPLICATION_H
