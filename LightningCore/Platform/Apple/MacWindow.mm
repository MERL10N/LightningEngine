//
// Created by Kian Marvi on 11/24/24.
//

#include "MacWindow.h"
#include <Metal/Metal.hpp>
#include <QuartzCore/QuartzCore.hpp>
#include <QuartzCore/QuartzCore.h>
#include "../../Renderer/Metal/MetalRenderer.h"
#include <Appkit/Appkit.h>

#define GLFW_INCLUDE_NONE
#import <GLFW/glfw3.h>
#define GLFW_EXPOSE_NATIVE_COCOA
#import <GLFW/glfw3native.h>

void MacWindow::frameBufferSizeCallback(GLFWwindow *window, int width, int height)
{
    MacWindow* engine = (MacWindow*)glfwGetWindowUserPointer(window);
    engine->resizeFrameBuffer(width, height);
}

void MacWindow::resizeFrameBuffer(int width, int height)
{
    m_MetalLayer->setDrawableSize(CGSizeMake(width, height));
}


MacWindow::MacWindow(unsigned int p_Width, unsigned int p_Height, const char *p_Title)
: m_Width(p_Width),
  m_Height(p_Height),
  m_Title(p_Title),
  m_MetalDevice(MTL::CreateSystemDefaultDevice())
{
    glfwInit();
    glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
    m_GlfwWindow = glfwCreateWindow(m_Width, m_Height, m_Title, nullptr, nullptr);
    NSWindow* m_MetalWindow = glfwGetCocoaWindow(m_GlfwWindow);
    m_MetalLayer = CA::MetalLayer::layer();
    m_MetalLayer->setDevice(m_MetalDevice);
    m_MetalLayer->setPixelFormat(MTL::PixelFormatRGBA8Unorm);
    m_MetalWindow.contentView.layer = (__bridge CAMetalLayer*)m_MetalLayer;
    m_MetalWindow.contentView.wantsLayer = YES;
    
    glfwSetWindowUserPointer(m_GlfwWindow, this);
    glfwSetFramebufferSizeCallback(m_GlfwWindow, frameBufferSizeCallback);
    
    m_MetalRenderer = new MetalRenderer(m_MetalDevice, m_MetalLayer);
}

void MacWindow::Update()
{
    while (!glfwWindowShouldClose(m_GlfwWindow))
    {
           m_MetalRenderer->BeginFrame();
           m_MetalRenderer->Render();
           m_MetalRenderer->Commit();
           glfwPollEvents();
    }
}

MacWindow::~MacWindow()
{
    
    if (m_MetalDevice)
    {
        m_MetalDevice->release();
        m_MetalDevice = nullptr;
    }

    glfwTerminate();
}
