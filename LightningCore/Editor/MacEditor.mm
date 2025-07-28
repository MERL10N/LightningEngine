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
  m_MetalRenderer(MetalRenderer(p_MetalKitView->device())),
  m_MetalFrameBuffer(MetalFrameBuffer(p_MetalKitView))
{
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
}

MacEditor::~MacEditor()
{
    ImGui_ImplMetal_Shutdown();
    ImGui_ImplOSX_Shutdown();
    ImGui::DestroyContext();
   
    /*
    if (m_MetalFrameBuffer)
    {
        delete m_MetalFrameBuffer;
        m_MetalFrameBuffer = nullptr;
    }
     */
}

void MacEditor::drawInMTKView(MTK::View* p_MetalKitView)
{

    ImGuiIO& io = ImGui::GetIO();
    ImGui_ImplMetal_NewFrame((__bridge MTLRenderPassDescriptor*)(p_MetalKitView->currentRenderPassDescriptor()));
    ImGui_ImplOSX_NewFrame((__bridge NSView*)(p_MetalKitView));
    ImGui::NewFrame();
    
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
    
    // Rendering
    ImGui::Render();

    
    // Render Game Viewport
    m_MetalRenderer.BeginFrame();
    m_MetalRenderer.Render(m_MetalFrameBuffer.GetRenderPassDescriptor());
    m_MetalRenderer.Commit(false);

    // Render ImGui UI
    m_MetalRenderer.BeginFrame();
    auto* m_CommandBuffer = m_MetalRenderer.GetMetalCommandBuffer();
    auto* m_CommandEncoder = m_CommandBuffer->renderCommandEncoder(p_MetalKitView->currentRenderPassDescriptor());
    m_MetalFrameBuffer.UpdateViewport(m_CommandEncoder);
   
    ImGui_ImplMetal_RenderDrawData(ImGui::GetDrawData(), (__bridge id<MTLCommandBuffer>)(m_CommandBuffer), (__bridge id<MTLRenderCommandEncoder>)(m_CommandEncoder));
    
   if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
   {
       ImGui::UpdatePlatformWindows();
       ImGui::RenderPlatformWindowsDefault();
   }

    m_CommandEncoder->endEncoding();
    m_CommandBuffer->presentDrawable(p_MetalKitView->currentDrawable());
    m_CommandBuffer->commit();
   
}



