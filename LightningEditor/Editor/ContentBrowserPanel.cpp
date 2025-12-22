//
//  ContentBrowserPanel.cpp
//  LightningEditor
//
//  Created by Kian Marvi on 12/21/25.
//

#include "ContentBrowserPanel.h"
#include "imgui.h"
#include <filesystem>

const char* s_AssetsDirectory = "../../../Assets";

void ContentBrowserPanel::Render()
{
    ImGui::Begin("Content Browser");
    
    for (auto &p : std::filesystem::directory_iterator(s_AssetsDirectory))
    {
        ImGui::Text("%s", p.path().string().c_str());
    }
    ImGui::End();
}
