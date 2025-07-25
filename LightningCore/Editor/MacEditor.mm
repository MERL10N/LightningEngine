//
//  MacEditor.mm
//  LightningCore
//
//  Created by Kian Marvi on 7/9/25.
//

#include <../imgui/backends/imgui_impl_metal.h>
#include <../imgui/backends/imgui_impl_osx.h>

#include <Metal/Metal.h>
#include <MetalKit/MetalKit.h>

#include <Metal/Metal.hpp>
#include <MetalKit/MetalKit.hpp>

#include "../Renderer/Metal/MetalFrameBuffer.h"


#include "MacEditor.h"
#include <iostream>

MacEditor::MacEditor(MTK::View* p_MetalKitView)
: ViewDelegate(),
  m_MetalRenderer(MetalRenderer(p_MetalKitView->device())),
  m_MetalFrameBuffer(new MetalFrameBuffer(p_MetalKitView))
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
    
    if (m_MetalFrameBuffer)
    {
        delete m_MetalFrameBuffer;
        m_MetalFrameBuffer = nullptr;
    }
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

    m_MetalRenderer.BeginFrame();
    
    ImGui::Begin("Game Scene");
    {
        ImVec2 viewportPanelSize = ImGui::GetContentRegionAvail();
      
        if (m_ViewportSize.x != viewportPanelSize.x || m_ViewportSize.y != viewportPanelSize.y)
        {
            m_ViewportSize.x = viewportPanelSize.x;
            m_ViewportSize.y = viewportPanelSize.y;
            m_MetalFrameBuffer->Resize(m_ViewportSize.x, m_ViewportSize.y);
        }
        ImGui::Image(m_MetalFrameBuffer->GetAttachmentTexture(), ImVec2{m_ViewportSize.x, m_ViewportSize.y}, ImVec2(1, 0), ImVec2(0, 1));
    }
    ImGui::End();
    
    
    m_MetalRenderer.Render(m_MetalFrameBuffer->GetRenderPassDescriptor());
    m_MetalRenderer.Commit(false);
    
     // Rendering
     ImGui::Render();

    m_MetalRenderer.BeginFrame();
    auto* p_ScreenPassDescriptor = p_MetalKitView->currentRenderPassDescriptor();
    auto* p_ImguiEncoder = m_MetalRenderer.GetMetalCommandBuffer()->renderCommandEncoder(p_ScreenPassDescriptor);
    m_MetalFrameBuffer->UpdateViewport(p_ImguiEncoder);
  
   
     ImGui_ImplMetal_RenderDrawData(ImGui::GetDrawData(),
     (__bridge id<MTLCommandBuffer>)(m_MetalRenderer.GetMetalCommandBuffer()),
     (__bridge id<MTLRenderCommandEncoder>)(p_ImguiEncoder));
    
    if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
    {
        ImGui::UpdatePlatformWindows();
        ImGui::RenderPlatformWindowsDefault();
    }
   p_ImguiEncoder->endEncoding();
   

}



