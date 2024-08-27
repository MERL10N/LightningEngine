//  Editor.cpp
//  LightningEngine
//  Created by Kian Marvi on 2/2/24.

#include "Editor.h"
#include <Metal/Metal.hpp>
#include <MetalKit/MetalKit.hpp>
#include <AppKit/AppKit.hpp>

#include <imgui.h>
#include <backends/imgui_impl_metal.h>
#include <backends/imgui_impl_osx.h>




bool CEditor::Init(MTL::Device* device, MTK::View* view)
{
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
    io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;         // Enable Docking
   
    // Setup style
    ImGui::StyleColorsDark();
    
    
    ImGui_ImplMetal_Init(device);

    // Setup Platform/Renderer backends
    ImGui_ImplOSX_Init(view);
    
   io.Fonts->AddFontDefault();
   mainFont = io.Fonts->AddFontFromFileTTF("assets/Fonts/NaturalMonoRegular-9YBeK.ttf", 16.f, nullptr, io.Fonts->GetGlyphRangesJapanese());
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
    ImGui::NewFrame();


    ImGui::PushFont(mainFont);
    
    // Dockspace code start
    
    static bool opt_fullscreen = true;
    static bool opt_padding = false;
    static ImGuiDockNodeFlags dockspace_flags = ImGuiDockNodeFlags_None;

    // We are using the ImGuiWindowFlags_NoDocking flag to make the parent window not dockable into,
    // because it would be confusing to have two docking targets within each others.
    ImGuiWindowFlags window_flags = ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDocking;
    if (opt_fullscreen)
    {
        const ImGuiViewport* viewport = ImGui::GetMainViewport();
        ImGui::SetNextWindowPos(viewport->WorkPos);
        ImGui::SetNextWindowSize(viewport->WorkSize);
        ImGui::SetNextWindowViewport(viewport->ID);
        ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
        ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
        window_flags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;
        window_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;
    }
    else
    {
        dockspace_flags &= ~ImGuiDockNodeFlags_PassthruCentralNode;
    }

    // When using ImGuiDockNodeFlags_PassthruCentralNode, DockSpace() will render our background
    // and handle the pass-thru hole, so we ask Begin() to not render a background.
    if (dockspace_flags & ImGuiDockNodeFlags_PassthruCentralNode)
        window_flags |= ImGuiWindowFlags_NoBackground;

    // Important: note that we proceed even if Begin() returns false (aka window is collapsed).
    // This is because we want to keep our DockSpace() active. If a DockSpace() is inactive,
    // all active windows docked into it will lose their parent and become undocked.
    // We cannot preserve the docking relationship between an active window and an inactive docking, otherwise
    // any change of dockspace/settings would lead to windows being stuck in limbo and never being visible.
    if (!opt_padding)
        ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
    ImGui::Begin("DockSpace Demo", &opt_fullscreen, window_flags);
    if (!opt_padding)
        ImGui::PopStyleVar();

    if (opt_fullscreen)
        ImGui::PopStyleVar(2);

    // Submit the DockSpace
    ImGuiIO& io = ImGui::GetIO();
    if (io.ConfigFlags & ImGuiConfigFlags_DockingEnable)
    {
        ImGuiID dockspace_id = ImGui::GetID("MyDockSpace");
        ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), dockspace_flags);
    }

    if (ImGui::BeginMenuBar())
    {
        if (ImGui::BeginMenu("File"))
        {
            // Disabling fullscreen would allow the window to be moved to the front of other windows,
            // which we can't undo at the moment without finer window depth/z control.
            ImGui::MenuItem("Fullscreen", NULL, &opt_fullscreen);
            ImGui::MenuItem("Padding", NULL, &opt_padding);
            ImGui::Separator();

        
            ImGui::EndMenu();
        }
 

        ImGui::EndMenuBar();
    }
    
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
    
    ImGui::Begin("Viewport");
    //ImVec2 viewportPanelSze = ImGui::GetContentRegionAvail();
    ImGui::End();
    

    ImGui::End();
    
    // Dockspace code end

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
