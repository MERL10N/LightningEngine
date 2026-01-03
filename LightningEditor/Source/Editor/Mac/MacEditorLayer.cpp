//
//  MacEditorLayer.cpp
//  LightningEditor
//
//  Created by Kian Marvi on 12/22/25.
//

#include "MacEditorLayer.h"
#include "imgui.h"
#include "Metal/Metal.hpp"

MacEditorLayer::MacEditorLayer(MTL::Device* p_MetalDevice)
: s_AssetsPath("Assets"),
  m_CurrentDirectory(s_AssetsPath),
  m_MetalDevice(p_MetalDevice),
  m_FolderIcon("Assets/Textures/foldericon.png", m_MetalDevice),
  m_FileIcon("Assets/Textures/file_icon.png", m_MetalDevice),
  m_ShaderIcon("Assets/Textures/Metal_4.png", m_MetalDevice),
  m_ReturnIcon("Assets/Textures/return_blue.png", m_MetalDevice)
{
}

MacEditorLayer::~MacEditorLayer()
{
    if (m_MetalDevice)
    {
        m_MetalDevice->release();
        m_MetalDevice = nullptr;
    }
}

void MacEditorLayer::DrawMenuBar()
{
    // TODO: Work on scene serialization
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
    ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0,0,0,0));
    
    if (m_CurrentDirectory != std::filesystem::path(s_AssetsPath))
    {
        if (ImGui::ImageButton("##ReturnButton", (ImTextureID)m_ReturnIcon.GetTexture(), ImVec2(48, 24),ImVec2(0,1), ImVec2(1, 0), ImVec4(0,0,0,0), ImVec4(1,1,1,1)))
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
        
        if (fileNameString.contains(".DS_Store"))
        {
            continue; // For macOS, ignore any .DS_Store files in the content browser
        }
        
        ImGui::PushID(fileNameString.c_str());
        
    
        if (directoryEntry.is_directory())
        {
            ImGui::ImageButton("##IconButton", (ImTextureID)m_FolderIcon.GetTexture(), ImVec2(96, 96),ImVec2(0,1), ImVec2(1, 0), ImVec4(0,0,0,0), ImVec4(1,1,1,1));
        }
        else
        {
            if (fileNameString.contains(".metal"))
            {
                ImGui::ImageButton("##FileIconButton", (ImTextureID)m_ShaderIcon.GetTexture(), ImVec2(96, 96),ImVec2(0,1), ImVec2(1, 0), ImVec4(0,0,0,0), ImVec4(1,1,1,1));
            }
            else
            {
                ImGui::ImageButton("##FileIconButton", (ImTextureID)m_FileIcon.GetTexture(), ImVec2(96, 96),ImVec2(0,1), ImVec2(1, 0), ImVec4(0,0,0,0), ImVec4(1,1,1,1));
            }
        }
        
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
    ImGui::PopStyleColor();
    ImGui::Columns(1);
    ImGui::End();
}

void MacEditorLayer::DrawStatsBar()
{
    ImGui::Begin("Stats");
    ImGui::Text("Frame Rate: (%.1f FPS)", ImGui::GetIO().Framerate);
    ImGui::Text("Frame Time: %.3f ms/frame", 1000.0f / ImGui::GetIO().Framerate);
    ImGui::Checkbox("Message from developer", &b_showAnotherWindow);
    if (b_showAnotherWindow)
    {
        ImGui::Begin("Message from developer", &b_showAnotherWindow);   // Pass a pointer to our bool variable (the window will have a closing button that will clear the bool when clicked)
        ImGui::Text("I can't wait to show you more!");
        if (ImGui::Button("Close"))
            b_showAnotherWindow = false;
        ImGui::End();
    }
    ImGui::End();
}
