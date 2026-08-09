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
        ImGui::Text("I can't wait to show you more of what is to come!");
        if (ImGui::Button("Close"))
            b_showAnotherWindow = false;
        ImGui::End();
    }
    
    
    ImGui::Checkbox("Enable Wireframe Mode ", &b_EnableWireFrameMode);
    ImGui::End();
}

