//
//  MacEditorLayer.h
//  LightningEditor
//
//  Created by Kian Marvi on 12/22/25.
//

#ifndef MacEditorLayer_h
#define MacEditorLayer_h

#include <filesystem>

namespace MTL
{
   class Device;
}

class MetalTexture;

class MacEditorLayer
{
public:
    MacEditorLayer(MTL::Device* p_MetalDevice);
    ~MacEditorLayer();
    
    void DrawStatsBar();
    void DrawMenuBar();
    void DrawContentBrowser();
    
private:
    bool b_showAnotherWindow;
    std::filesystem::path m_CurrentDirectory;
    MetalTexture* m_FolderIcon, *m_FileIcon;
    MTL::Device* m_MetalDevice;
};

#endif /* MacEditorLayer_hpp */
