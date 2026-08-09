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
#include "../EditorLayer.h"
#include "../ContentBrowserPanel.h"
#include <imgui.h>

namespace MTL
{
   class Device;
}

class MetalTexture;

class MacEditorLayer : public EditorLayer
{
public:
    MacEditorLayer() = default;
    MacEditorLayer(MTL::Device* metalDevice);
    ~MacEditorLayer();
    
    void DrawContentBrowserImpl();
    
private:
    const char*                m_AssetsPath         = nullptr;
    std::filesystem::path      m_CurrentDirectory;
    MetalTexture               m_FolderIcon, m_FileIcon, m_ShaderIcon, m_ReturnIcon;
    ContentBrowserPanel        m_ContentBrowserPanel;
    
};

#endif /* MacEditorLayer_hpp */
