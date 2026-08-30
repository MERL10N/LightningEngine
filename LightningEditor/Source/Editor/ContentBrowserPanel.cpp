//
//  ContentBrowserPanel.cpp
//  LightningEditor
//
//  Created by Kian Marvi on 12/21/25.
//

#include "ContentBrowserPanel.h"
#include <string>

static const char* s_AssetsPath = "Assets";

ContentBrowserPanel::ContentBrowserPanel()
: m_CurrentDirectory(s_AssetsPath)
{
}

ContentBrowserPanel::~ContentBrowserPanel()
{
}

void ContentBrowserPanel::Render(const std::array<ImTextureID, 5> &icons)
{
    
    ImGui::Begin("Content Browser");
    ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0,0,0,0));
    
    if (m_CurrentDirectory != std::filesystem::path(s_AssetsPath))
    {
        if (ImGui::ImageButton("##ReturnButton", icons[3], ImVec2(48, 24),ImVec2(0,1), ImVec2(1, 0), ImVec4(0,0,0,0), ImVec4(1,1,1,1)))
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
    
    for (const auto &directoryEntry : std::filesystem::directory_iterator(m_CurrentDirectory))
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
            ImGui::ImageButton("##IconButton", icons[0], ImVec2(96, 96),ImVec2(0,1), ImVec2(1, 0), ImVec4(0,0,0,0), ImVec4(1,1,1,1));
        }
        else
        {
            if (fileNameString.contains(".metal"))
            {
                ImGui::ImageButton("##FileIconButton", icons[2], ImVec2(96, 96),ImVec2(0,1), ImVec2(1, 0), ImVec4(0,0,0,0), ImVec4(1,1,1,1));
            }
            else if (fileNameString.contains(".png") || fileNameString.contains(".jpg"))
            {
                ImGui::ImageButton("##FileImageIconButton", icons[4], ImVec2(96, 96),ImVec2(0,1), ImVec2(1, 0), ImVec4(0,0,0,0), ImVec4(1,1,1,1));
            }
            else
            {
                ImGui::ImageButton("##FileIconButton", icons[1], ImVec2(96, 96),ImVec2(0,1), ImVec2(1, 0), ImVec4(0,0,0,0), ImVec4(1,1,1,1));
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
