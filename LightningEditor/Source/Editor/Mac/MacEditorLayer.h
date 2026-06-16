//
//  MacEditorLayer.h
//  LightningEditor
//
//  Created by Kian Marvi on 12/22/25.
//

#ifndef MacEditorLayer_h
#define MacEditorLayer_h

#include <filesystem>
#include "Renderer/Metal/MetalTexture.h"

namespace MTL
{
   class Device;
}

class MetalTexture;

class MacEditorLayer
{
public:
    MacEditorLayer() = default;
    MacEditorLayer(MTL::Device* p_MetalDevice);
    ~MacEditorLayer();
    
    void DrawStatsBar();
    void DrawMenuBar();
    void DrawContentBrowser();
    
    inline bool IsWireFrameEnabled() const { return b_EnableWireframe; }
    
private:
    bool b_showAnotherWindow;
    bool b_EnableWireframe = false;
    const char* s_AssetsPath = nullptr;
    std::filesystem::path m_CurrentDirectory;
    MTL::Device* m_MetalDevice;
    MetalTexture m_FolderIcon, m_FileIcon, m_ShaderIcon, m_ReturnIcon;
};

#endif /* MacEditorLayer_hpp */
