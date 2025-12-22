//
//  ContentBrowserPanel.h
//  LightningEditor
//
//  Created by Kian Marvi on 12/21/25.
//

#ifndef ContentBrowserPanel_h
#define ContentBrowserPanel_h

#include <filesystem>

class ContentBrowserPanel
{
public:
    ContentBrowserPanel();
    ~ContentBrowserPanel();
    
    void Render();
private:
    std::filesystem::path m_CurrentDirectory;
    
};
#endif /* ContentBrowserPanel_hpp */
