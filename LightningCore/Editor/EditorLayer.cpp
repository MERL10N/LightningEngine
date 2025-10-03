//
//  EditorLayer.cpp
//  LightningCore
//
//  Created by Kian Marvi on 8/2/25.
//

#include "EditorLayer.h"
#include "imgui.h"

EditorLayer::EditorLayer()
{
    b_showAnotherWindow = true;
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

void EditorLayer::DrawContentBrowser()
{
    ImGui::Begin("File explorer");
    ImGui::Text("Coming when it's ready");
    ImGui::PopFont();
    ImGui::End();
}

void EditorLayer::DrawStatsBar()
{
    ImGui::Begin("Welcome to Lightning Engine!");
    ImGui::Text("This is a metal game engine written in C++");
    ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
    ImGui::Checkbox("Message from developer", &b_showAnotherWindow);
    if (b_showAnotherWindow)
    {
        ImGui::Begin("Message from developer", &b_showAnotherWindow);   // Pass a pointer to our bool variable (the window will have a closing button that will clear the bool when clicked)
        ImGui::Text("I can't wait to show you more of what is to come!");
        if (ImGui::Button("Close"))
            b_showAnotherWindow = false;
        ImGui::End();
    }
    ImGui::End();
}

