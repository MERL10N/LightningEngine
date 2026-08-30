//
//  ContentBrowserPanel.h
//  LightningEditor
//
//  Created by Kian Marvi on 12/21/25.
//

#ifndef ContentBrowserPanel_h
#define ContentBrowserPanel_h

#include <filesystem>
#include <imgui.h>
#include <array>

class ContentBrowserPanel
{
public:
    ContentBrowserPanel();
    ~ContentBrowserPanel();
    
    void Render(const std::array<ImTextureID, 5> &icons);
private:
    std::filesystem::path m_CurrentDirectory;
};
#endif /* ContentBrowserPanel_hpp */
