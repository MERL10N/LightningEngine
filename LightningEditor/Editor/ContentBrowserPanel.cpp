//
//  ContentBrowserPanel.cpp
//  LightningEditor
//
//  Created by Kian Marvi on 12/21/25.
//

#include "ContentBrowserPanel.h"
#include "imgui.h"
#include <string>

static const char* s_AssetsPath = "../../../Assets";

ContentBrowserPanel::ContentBrowserPanel()
: m_CurrentDirectory(s_AssetsPath)
{
}

ContentBrowserPanel::~ContentBrowserPanel()
{
}

void ContentBrowserPanel::Render()
{
    ImGui::Begin("Content Browser");
    
    if (m_CurrentDirectory != std::filesystem::path(s_AssetsPath))
    {
        if (ImGui::Button("<-"))
        {
            m_CurrentDirectory = m_CurrentDirectory.parent_path();
        }
    }
    
    for (auto &directoryEntry : std::filesystem::directory_iterator(m_CurrentDirectory))
    {
        const auto &path = directoryEntry.path();
        auto relativePath = std::filesystem::relative(path, s_AssetsPath);
        std::string fileNameString = relativePath.filename().string();
        if (directoryEntry.is_directory())
        {
            if (ImGui::Button(fileNameString.c_str()))
            {
                m_CurrentDirectory /= path.filename();
            }
        }
        else
        {
            if (ImGui::Button(fileNameString.c_str()))
            {
            }
        }
    }
    ImGui::End();
}
