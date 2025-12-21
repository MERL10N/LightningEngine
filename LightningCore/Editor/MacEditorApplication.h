//
//  EditorApplication.hpp
//  LightningEditor
//
//  Created by Kian Marvi on 7/11/25.
//

#ifndef EditorApplication_hpp
#define EditorApplication_hpp

#include "EditorLayer.h"
#include "Platform/Apple/MacWindow.h"
#include <simd/simd.h>

class MetalRenderer;
class MetalFrameBuffer;

namespace MTL
{
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
    explicit MacEditorApplication(float p_Width = 1920.f, float p_Height = 1080.f, const char* p_Title = "Lightning Editor");
    void DrawGameViewport();
    ~MacEditorApplication();
    void Update();

private:

   // EditorLayer m_EditorLayer;
    MacWindow m_MacWindow;
    MetalRenderer* m_MetalRenderer = nullptr;
    CA::MetalDrawable* m_WindowDrawable = nullptr;
    MetalFrameBuffer* m_MetalFrameBuffer = nullptr;
    MTL::RenderPassDescriptor* m_WindowPassDescriptor = nullptr;
    MTL::CommandBuffer* m_ImGuiCommandBuffer = nullptr;
    MTL::RenderCommandEncoder* m_ImGuiCommandEncoder = nullptr;
    simd::float2 m_ViewportSize;
};

#endif /* EditorApplication_hpp */
