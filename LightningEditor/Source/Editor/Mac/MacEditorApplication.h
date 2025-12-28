//
//  EditorApplication.hpp
//  LightningEditor
//
//  Created by Kian Marvi on 7/11/25.
//

#ifndef EditorApplication_hpp
#define EditorApplication_hpp
#include "Platform/Apple/MacWindow.h"
#include "Input/AppleController.h"
#include "Camera/Camera.h"
#include <simd/simd.h>

class MetalRenderer;
class MetalFrameBuffer;
class MacEditorLayer;
class Scene;

namespace MTL
{
    class Device;
    class RenderPassDescriptor;
    class RenderCommandEncoder;
    class CommandBuffer;
}

namespace CA
{
  class MetalDrawable;
}

class MacEditorApplication
{
public:
    explicit MacEditorApplication(float p_Width = 1280.f, float p_Height = 720.f, const char* p_Title = "Lightning Editor");
    void DrawGameViewport();
    ~MacEditorApplication();
    void Update();

private:
    MacWindow m_MacWindow;
    MacEditorLayer* m_MacEditorLayer = nullptr;
    MetalRenderer* m_MetalRenderer = nullptr;
    CA::MetalDrawable* m_WindowDrawable = nullptr;
    MetalFrameBuffer* m_MetalFrameBuffer = nullptr;
    MTL::RenderPassDescriptor* m_WindowPassDescriptor = nullptr;
    MTL::CommandBuffer* m_ImGuiCommandBuffer = nullptr;
    MTL::RenderCommandEncoder* m_ImGuiCommandEncoder = nullptr;
    simd::float2 m_ViewportSize;
    
    Camera m_Camera;
    
    float m_AspectRatio = 0.0f;
    
    float m_CurrentFrame = 0.0f;
    float m_DeltaTime = 0.0f;
    float m_LastFrame = 0.0f;
    
    AppleController m_Controller;
    
};

#endif /* EditorApplication_hpp */
