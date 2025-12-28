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
class Scene;

class MacEditorLayer
{
public:
    MacEditorLayer() = default;
    MacEditorLayer(MTL::Device* p_MetalDevice);
    ~MacEditorLayer();
    
    void DrawStatsBar();
    void DrawMenuBar();
    void DrawContentBrowser();
    
private:
    bool b_showAnotherWindow;
    std::filesystem::path m_CurrentDirectory;
    MetalTexture m_FolderIcon, m_FileIcon, m_ShaderIcon, m_ReturnIcon;
    MTL::Device* m_MetalDevice;
};

#endif /* MacEditorLayer_hpp */
