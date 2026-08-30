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
#include "Camera/CameraController.h"
#include "Scene/Scene.h"
#include "Entity/Entity.h"
#include "Renderer/Metal/MetalFrameBuffer.h"
#include "Renderer/Metal/MetalRenderer.h"
#include "MacEditorLayer.h"
#include <imgui/imgui.h>

namespace MTL4
{
    class Device;
    class RenderPassDescriptor;
    class RenderCommandEncoder;
    class CommandBuffer;
}

namespace MTL
{
    class Drawable;
}

namespace CA
{
  class MetalDrawable;
}

class MacEditorApplication
{
public:
    explicit MacEditorApplication(const float p_Width, const float p_Height, const char* p_Title = "Lightning Editor");
    
    void DrawGameViewport();
    ~MacEditorApplication();
    void Update();

private:
    MacWindow m_MacWindow;
    
    MTL::Device*                    m_MetalDevice           = nullptr;
    MTL4::RenderPassDescriptor*     m_WindowPassDescriptor  = nullptr;
    MTL4::CommandAllocator*         m_ImGuiCommandAllocator = nullptr;
    MTL4::CommandBuffer*            m_ImGuiCommandBuffer    = nullptr;
    MTL4::RenderCommandEncoder*     m_ImGuiCommandEncoder   = nullptr;
    MTL::Drawable*                  m_Drawable              = nullptr;
    
    MacEditorLayer       m_MacEditorLayer;
    MetalRenderer        m_MetalRenderer;
    MetalFrameBuffer     m_MetalFrameBuffer;
    
    CA::MetalDrawable*   m_WindowDrawable = nullptr;
    
    ImVec2 m_ViewportSize;
    
    CameraController m_CameraController;
    
    float m_AspectRatio = 0.0f;
    float m_CurrentFrame = 0.0f;
    float m_DeltaTime = 0.0f;
    float m_LastFrame = 0.0f;
    float m_Width = 0.0f;
    float m_Height = 0.0f;
    
    AppleController m_Controller;
    
    Scene  m_Scene;
    Entity m_CameraEntity;
    
    bool b_EnableWireframe = false;
    
};

#endif /* EditorApplication_hpp */

