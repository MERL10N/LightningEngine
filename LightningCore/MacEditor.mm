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

MacEditor::MacEditor(MTK::View* p_MetalKitView)
: ViewDelegate(),
  m_MetalRenderer(MetalRenderer(p_MetalKitView->device())),
  show_another_window(false)
{
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO();(void)io;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
    io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;         // Enable Docking
    io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;       // Enable Multi-Viewport / Platform Windows
    
    ImGui::StyleColorsDark();
    

    ImGui_ImplMetal_Init((__bridge id<MTLDevice>)(p_MetalKitView->device()));
    ImGui_ImplOSX_Init(((__bridge NSView*)(p_MetalKitView)));
    // TODO: Implement initialising ImGui UI for LightningEditor
    io.Fonts->AddFontDefault();
}

MacEditor::~MacEditor()
{
    ImGui_ImplMetal_Shutdown();
    ImGui_ImplOSX_Shutdown();
    ImGui::DestroyContext();
}

void MacEditor::drawInMTKView(MTK::View* p_MetalKitView)
{
    
    ImGui_ImplMetal_NewFrame((__bridge MTLRenderPassDescriptor*)(p_MetalKitView->currentRenderPassDescriptor()));
    ImGui_ImplOSX_NewFrame((__bridge NSView*)(p_MetalKitView));
    ImGui::NewFrame();
    
    // TODO: Implement Rendering ImGui UI elements for LightningEditor
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
    
    
    m_MetalRenderer.Render(p_MetalKitView);
    
    
     // Rendering
     ImGui::Render();
     ImGui_ImplMetal_RenderDrawData(ImGui::GetDrawData(),
     (__bridge id<MTLCommandBuffer>)(m_MetalRenderer.GetMetalCommandBuffer()),
     (__bridge id<MTLRenderCommandEncoder>)(m_MetalRenderer.GetMetalRenderCommandEncoder()));
    
     m_MetalRenderer.Commit();
    
    ImGuiIO& io = ImGui::GetIO();(void)io;
    // Update and Render additional Platform Windows
    if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
    {
        ImGui::UpdatePlatformWindows();
        ImGui::RenderPlatformWindowsDefault();
    }
}



