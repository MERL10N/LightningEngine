//
//  MacEditor.mm
//  LightningCore
//
//  Created by Kian Marvi on 7/9/25.
//

#include <../imgui/imgui.h>
#include <../imgui/backends/imgui_impl_metal.h>
#include <../imgui/backends/imgui_impl_osx.h>

#include <Metal/Metal.h>
#include <MetalKit/MetalKit.h>

#include <Metal/Metal.hpp>
#include <MetalKit/MetalKit.hpp>


#include "MacEditor.h"
#include <iostream>


MacEditor::MacEditor(MTK::View* p_MetalKitView)
: ViewDelegate(),
  m_MetalRenderer(MetalRenderer(p_MetalKitView->device(), MTL::PixelFormatDepth32Float)),
  m_MetalFrameBuffer(MetalFrameBuffer(p_MetalKitView))
{
    assert(p_MetalKitView != nullptr);
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO();
    (void)io;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
    io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;         // Enable Docking
    io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;       // Enable viewports
    
    ImGui::StyleColorsDark();

    ImGui_ImplMetal_Init((__bridge id<MTLDevice>)(p_MetalKitView->device()));
    ImGui_ImplOSX_Init(((__bridge NSView*)(p_MetalKitView)));
    io.Fonts->AddFontDefault();
    
    m_MetalFrameBuffer.Create(p_MetalKitView->drawableSize().width, p_MetalKitView->drawableSize().height);
}

MacEditor::~MacEditor()
{
    ImGui_ImplMetal_Shutdown();
    ImGui_ImplOSX_Shutdown();
    ImGui::DestroyContext();
}

void MacEditor::drawInMTKView(MTK::View* p_MetalKitView)
{
    assert(p_MetalKitView);

    ImGuiIO& io = ImGui::GetIO();
    ImGui_ImplMetal_NewFrame((__bridge MTLRenderPassDescriptor*)(p_MetalKitView->currentRenderPassDescriptor()));
    ImGui_ImplOSX_NewFrame((__bridge NSView*)(p_MetalKitView));
    ImGui::NewFrame();
    
    
    ImGui::DockSpaceOverViewport();
    ImGui::Begin("Welcome to Lightning Engine!");
    ImGui::Text("This is a metal game engine written in C++");
    ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
    ImGui::Checkbox("Message from developer", &show_another_window);
    if (show_another_window)
    {
        ImGui::Begin("Message from developer", &show_another_window);   // Pass a pointer to our bool variable (the window will have a closing button that will clear the bool when clicked)
        ImGui::Text("I can't wait to show you more of what is to come!");
        if (ImGui::Button("Close"))
            show_another_window = false;
        ImGui::End();
    }
    ImGui::End();
    
    ImGui::Begin("File explorer");
    ImGui::Text("Coming when it's ready");
    ImGui::End();
    
    // Prevent crashes when compiling for the first time
    if (m_ViewportSize.x == 0.0f || m_ViewportSize.y == 0.0f)
        ImGui::SetNextWindowSize(ImVec2(1920, 1200), ImGuiCond_FirstUseEver);
    
    ImGui::Begin("Game Scene");
    {
        ImVec2 viewportPanelSize = ImGui::GetContentRegionAvail();
      
        if (m_ViewportSize.x != viewportPanelSize.x || m_ViewportSize.y != viewportPanelSize.y)
        {
            m_ViewportSize.x = viewportPanelSize.x;
            m_ViewportSize.y = viewportPanelSize.y;
            m_MetalFrameBuffer.Resize(m_ViewportSize.x, m_ViewportSize.y);
        }
        ImGui::Image(m_MetalFrameBuffer.GetAttachmentTexture(), ImVec2{m_ViewportSize.x, m_ViewportSize.y}, ImVec2(1, 0), ImVec2(0, 1));
    }
    ImGui::End();
    
    if (ImGui::BeginMainMenuBar()) {
                  if (ImGui::BeginMenu("File")) {
                       if (ImGui::MenuItem("New Scene")) {
                       }
                       if (ImGui::MenuItem("Open Scene", "Command+O")) {
                       }
                       if (ImGui::MenuItem("Save", "Command+S")) {
                       }
                       if (ImGui::MenuItem("Save as..")) {
                        }
                 ImGui::EndMenu();
                 }
                 ImGui::EndMainMenuBar();
            }
    
    // Rendering
    ImGui::Render();

    
    // Submit Framebuffer to Renderer
    m_MetalRenderer.BeginFrame();
    m_MetalRenderer.Render(m_MetalFrameBuffer.GetRenderPassDescriptor());
    m_MetalRenderer.Commit();

    // Render ImGui UI and Viewport
     m_MetalRenderer.BeginFrame();
    auto* m_ImGuiCommandBuffer = m_MetalRenderer.GetMetalCommandBuffer();
    auto* m_ImGuiCommandEncoder = m_ImGuiCommandBuffer->renderCommandEncoder(p_MetalKitView->currentRenderPassDescriptor());
    m_MetalFrameBuffer.UpdateViewport(m_ImGuiCommandEncoder);

    ImGui_ImplMetal_RenderDrawData(ImGui::GetDrawData(), (__bridge id<MTLCommandBuffer>)(m_ImGuiCommandBuffer), (__bridge id<MTLRenderCommandEncoder>)(m_ImGuiCommandEncoder));

    if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
    {
    ImGui::UpdatePlatformWindows();
    ImGui::RenderPlatformWindowsDefault();
    }

    m_ImGuiCommandEncoder->endEncoding();
    m_ImGuiCommandBuffer->presentDrawable(p_MetalKitView->currentDrawable());
    m_ImGuiCommandBuffer->commit();
   
}



