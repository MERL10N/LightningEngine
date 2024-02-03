//  Editor.cpp
//  LightningEngine
//  Created by Kian Marvi on 2/2/24.

#include "CEditor.h"

#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>

#define IMGUI_IMPL_METAL_CPP
#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_metal.h"


bool CEditor::Init(MTL::Device* device, GLFWwindow* window)
{
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;


    // Setup style
    ImGui::StyleColorsDark();
    
    
    ImGui_ImplMetal_Init(device);

    // Setup Platform/Renderer backends
    ImGui_ImplGlfw_InitForOther(window, true);
    
    
    show_demo_window = true;
    show_another_window = false;

    return true;
}

void CEditor::SetClearColor(float value, int index)
{
    clear_color[index] = value;
}

float CEditor::GetClearColor(int index) const
{
    return clear_color[index];
}

void CEditor::Render(MTL::RenderPassDescriptor *renderPassDescriptor, MTL::CommandBuffer *metalCommandBuffer, MTL::RenderCommandEncoder *renderCommandEncoder)
{
    // Start the Dear ImGui frame
    ImGui_ImplMetal_NewFrame(renderPassDescriptor);
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();

    // 1. Show the big demo window (Most of the sample code is in ImGui::ShowDemoWindow()! You can browse its code to learn more about Dear ImGui!).
    if (show_demo_window)
        ImGui::ShowDemoWindow(&show_demo_window);

    {

        ImGui::Begin("Welcome to Lightning Engine!");                          // Create a window called "Hello, world!" and append into it.

        ImGui::Text("This is a metal game engine written in C++");               // Display some text (you can use a format strings too)
        ImGui::Checkbox("Demo Window", &show_demo_window);      // Edit bools storing our window open/close state
        ImGui::Checkbox("Message from developer", &show_another_window);
        

        ImGui::ColorEdit3("Adjust color", (float*)&clear_color); // Edit 3 floats representing a color
    
    
        ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
        ImGui::End();
    }

    // 3. Show another simple window.
    if (show_another_window)
    {
        ImGui::Begin("Message from developer", &show_another_window);   // Pass a pointer to our bool variable (the window will have a closing button that will clear the bool when clicked)
        ImGui::Text("I can't wait to show you more of what is to come!");
        if (ImGui::Button("Close"))
            show_another_window = false;
        ImGui::End();
    }

    // Rendering
    ImGui::Render();
    ImGui_ImplMetal_RenderDrawData(ImGui::GetDrawData(), metalCommandBuffer, renderCommandEncoder);
}

void CEditor::Destroy()
{
    ImGui_ImplMetal_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
}
