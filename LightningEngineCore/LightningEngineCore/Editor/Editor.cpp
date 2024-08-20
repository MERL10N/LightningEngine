//  Editor.cpp
//  LightningEngine
//  Created by Kian Marvi on 2/2/24.

#include "Editor.h"
#include <Metal/Metal.hpp>
#include <MetalKit/MetalKit.hpp>
#include <AppKit/AppKit.hpp>

#define IMGUI_IMPL_METAL_CPP
#define IMGUI_IMPL_METAL_CPP_EXTENSIONS
#include "imgui.h"
#include "imgui_impl_metal.h"
#include "imgui_impl_osx.h"




bool CEditor::Init(MTL::Device* device, MTK::View* view)
{
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;

    // Setup style
    ImGui::StyleColorsDark();
    
    
    ImGui_ImplMetal_Init(device);

    // Setup Platform/Renderer backends
    ImGui_ImplOSX_Init(view);
    
    io.Fonts->AddFontDefault();
    mainFont = io.Fonts->AddFontFromFileTTF("assets/Fonts/AovelSansRounded-rdDL.ttf", 16.f, nullptr, io.Fonts->GetGlyphRangesJapanese());
    assert(mainFont != nullptr);
    
    
    show_demo_window = true;
    show_another_window = false;
    fps = 120;

    return true;
}

void CEditor::SetClearColor(float value, int index)
{
    clear_color[index] = value;
}

void CEditor::SetFrameRate(int fps)
{
    this->fps = fps;
}

float CEditor::GetClearColor(int index) const
{
    return clear_color[index];
}

int CEditor::GetFrameRate()
{
    return fps;
}

void CEditor::Render(MTL::RenderPassDescriptor *renderPassDescriptor, MTL::CommandBuffer *metalCommandBuffer, MTL::RenderCommandEncoder *renderCommandEncoder, MTK::View* view)
{
    // Start the Dear ImGui frame
    ImGui_ImplMetal_NewFrame(renderPassDescriptor);
    ImGui_ImplOSX_NewFrame(view);
    void BeginFrame();
    ImGui::NewFrame();


    ImGui::PushFont(mainFont);
    ImGui::Begin("Welcome to Lightning Engine!");                          // Create a window called "Hello, world!" and append into it.
    

    ImGui::Text("This is a metal game engine written in C++");               // Display some text (you can use a format strings too)

    ImGui::Checkbox("Message from developer", &show_another_window);
    
    ImGui::Checkbox("Enable 120fps", &enable_high_fps);
    
    if (enable_high_fps)
    {
        fps = 120;
    }
    else
    {
        fps = 60;
    }

    ImGui::ColorEdit3("Adjust color", (float*)&clear_color); // Edit 3 floats representing a color


    ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
    
    
    ImGui::End();


    // 3. Show another simple window.
    if (show_another_window)
    {
        ImGui::Begin("Message from developer", &show_another_window);   // Pass a pointer to our bool variable (the window will have a closing button that will clear the bool when clicked)
        ImGui::Text("I can't wait to show you more of what is to come!");
        if (ImGui::Button("Close"))
            show_another_window = false;
        ImGui::End();
    }
    ImGui::PopFont();
        // Rendering
        ImGui::Render();
        ImGui_ImplMetal_RenderDrawData(ImGui::GetDrawData(), metalCommandBuffer, renderCommandEncoder);
}

void CEditor::Destroy()
{
    ImGui_ImplMetal_Shutdown();
    ImGui_ImplOSX_Shutdown();
    ImGui::DestroyContext();
}
