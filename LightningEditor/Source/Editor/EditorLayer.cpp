//
//  EditorLayer.cpp
//  LightningCore
//
//  Created by Kian Marvi on 8/2/25.
//

#include "EditorLayer.h"
#include "ContentBrowserPanel.h"

ContentBrowserPanel contentPanel;

EditorLayer::EditorLayer()
:  b_showAnotherWindow(true),
   b_EnableWireFrameMode(false)
{
}

EditorLayer::~EditorLayer()
{
}


void EditorLayer::DrawMenuBar()
{
    if (ImGui::BeginMainMenuBar())
    {
        if (ImGui::BeginMenu("File"))
        {
            if (ImGui::MenuItem("New Scene"))
            {
            }
            if (ImGui::MenuItem("Open Scene", "Command+O"))
            {
            }
            if (ImGui::MenuItem("Save", "Command+S"))
            {
            }
            if (ImGui::MenuItem("Save as.."))
            {
            }
            ImGui::EndMenu();
        }
        ImGui::EndMainMenuBar();
    }
}

void EditorLayer::DrawStatsBar()
{
    ImGui::Begin("Statistics");
    ImGui::Text("Performance: %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
    ImGui::Checkbox("Message from developer", &b_showAnotherWindow);
    if (b_showAnotherWindow)
    {
        ImGui::Begin("Message from developer", &b_showAnotherWindow);
        ImGui::Text("There is more to come!");
        if (ImGui::Button("Close"))
            b_showAnotherWindow = false;
        ImGui::End();
    }
    
    
    ImGui::Checkbox("Enable Wireframe Mode ", &b_EnableWireFrameMode);
    ImGui::End();
}

void EditorLayer::DarkModeTheme()
{
    ImGui::CreateContext();
    auto& colors = ImGui::GetStyle().Colors;
    colors[ImGuiCol_WindowBg] = {0.1f, 0.105f, 0.11f, 1.0f};
    
    // Headers
    colors[ImGuiCol_Header] = {0.2f, 0.205f, 0.21f, 1.0f};
    colors[ImGuiCol_HeaderHovered] = {0.3f, 0.305f, 0.31f, 1.0f};
    colors[ImGuiCol_HeaderActive] = {0.15f, 0.1505f, 0.151f, 1.0f};
    
    // Buttons
    colors[ImGuiCol_Button] = {0.2f, 0.205f, 0.21f, 1.0f};
    colors[ImGuiCol_ButtonHovered] = {0.3f, 0.305f, 0.31f, 1.0f};
    colors[ImGuiCol_ButtonActive] = {0.15f, 0.1505f, 0.151f, 1.0f};
    
    // Frame BG
    colors[ImGuiCol_FrameBg] = {0.2f, 0.205f, 0.21f, 1.0f};
    colors[ImGuiCol_FrameBgHovered] = {0.3f, 0.305f, 0.31f, 1.0f};
    colors[ImGuiCol_FrameBgActive] = {0.15f, 0.1505f, 0.151f, 1.0f};
    
    // Tabs
    colors[ImGuiCol_Tab] = {0.15f, 0.1505f, 0.151f, 1.0f};
    colors[ImGuiCol_TabHovered] = {0.38f, 0.3805f, 0.381f, 1.0f};
    colors[ImGuiCol_TabActive] = {0.28f, 0.2805f, 0.281f, 1.0f};
    colors[ImGuiCol_TabUnfocused] = {0.15f, 0.1505f, 0.151f, 1.0f};
    colors[ImGuiCol_TabUnfocusedActive] = {0.2f, 0.205f, 0.21f, 1.0f};
    
    // Title
    colors[ImGuiCol_TitleBg] = {0.15f, 0.1505f, 0.151f, 1.0f};
    colors[ImGuiCol_TitleBgActive] = {0.15f, 0.1505f, 0.151f, 1.0f};
    colors[ImGuiCol_TitleBgCollapsed] = {0.95f, 0.9505f, 0.951f, 1.0f};
}
