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

class MacEditorLayer final : public EditorLayer
{
public:
    MacEditorLayer() = default;
    MacEditorLayer(MTL::Device* metalDevice);
    ~MacEditorLayer();
    
    virtual void DrawContentBrowser() override;
    
private:
    const char*                m_AssetsPath         = nullptr;
    std::filesystem::path      m_CurrentDirectory;
    MetalTexture               m_FolderIcon, m_FileIcon, m_ShaderIcon, m_ReturnIcon, m_FileImageIcon;
    ContentBrowserPanel        m_ContentBrowserPanel;
    
};

#endif /* MacEditorLayer_hpp */
