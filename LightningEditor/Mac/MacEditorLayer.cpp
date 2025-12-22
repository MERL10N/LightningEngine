//
//  MacEditorLayer.cpp
//  LightningEditor
//
//  Created by Kian Marvi on 12/22/25.
//

#include "MacEditorLayer.h"
#include "imgui.h"
#include "Renderer/Metal/MetalTexture.h"
#include "Metal/Metal.hpp"
#include <print>
static const char* s_AssetsPath = "Assets";

MacEditorLayer::MacEditorLayer(MTL::Device* p_MetalDevice)
: m_CurrentDirectory(s_AssetsPath),
  m_MetalDevice(p_MetalDevice),
  m_Icon(new MetalTexture("Assets/Textures/foldericon.png"))
{
    m_Icon->SetMetalDevice(m_MetalDevice);
}

MacEditorLayer::~MacEditorLayer()
{
    std::println("MacEditorLayer destructor called");
    if (m_Icon)
    {
        delete m_Icon;
        m_Icon = nullptr;
    }
    if (m_MetalDevice)
    {
        m_MetalDevice->release();
        m_MetalDevice = nullptr;
    }
}

void MacEditorLayer::DrawMenuBar()
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

void MacEditorLayer::DrawContentBrowser()
{
    ImGui::Begin("Content Browser");
    
    if (m_CurrentDirectory != std::filesystem::path(s_AssetsPath))
    {
        if (ImGui::Button("Go back"))
        {
            m_CurrentDirectory = m_CurrentDirectory.parent_path();
        }
    }
    
    static float padding = 16.0f;
    static float thumbnailSize = 128.f;
    float ceilSize = thumbnailSize + padding;
    
    float panelWidth = ImGui::GetContentRegionAvail().x;
    int columnCount = (int)(panelWidth/ceilSize);
    
    if (columnCount < 1)
        columnCount = 1;
    
    ImGui::Columns(columnCount,0,false);
    
    for (auto &directoryEntry : std::filesystem::directory_iterator(m_CurrentDirectory))
    {
        
        const auto &path = directoryEntry.path();
        auto relativePath = std::filesystem::relative(path, s_AssetsPath);
        std::string fileNameString = relativePath.filename().string();
        ImGui::PushID(fileNameString.c_str());
        ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0,0,0,0));
        ImGui::ImageButton("##IconButton", (ImTextureID)m_Icon->GetTexture(), ImVec2(32, 32),ImVec2(0,1), ImVec2(1, 0), ImVec4(0,0,0,0), ImVec4(1,1,1,1));
        ImGui::PopStyleColor();
        if (ImGui::IsItemHovered() && ImGui::IsMouseDoubleClicked(ImGuiMouseButton_Left))
        {
            if (directoryEntry.is_directory())
            {
                    m_CurrentDirectory /= path.filename();
            }
        }
        ImGui::PopID();
        ImGui::TextWrapped("%s",fileNameString.c_str());
        ImGui::NextColumn();
    }
    ImGui::Columns(1);
    ImGui::End();
}

void MacEditorLayer::DrawStatsBar()
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
