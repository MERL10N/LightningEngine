//
//  MacEditorLayer.cpp
//  LightningEditor
//
//  Created by Kian Marvi on 12/22/25.
//

#include "MacEditorLayer.h"
#include <imgui/imgui.h>
#include <Metal/Metal.hpp>

MacEditorLayer::MacEditorLayer(MTL::Device* metalDevice)
: m_AssetsPath("Assets"),
  m_CurrentDirectory(m_AssetsPath),
  m_FolderIcon("Assets/Textures/foldericon.png", metalDevice),
  m_FileIcon("Assets/Textures/file_icon.png", metalDevice),
  m_ShaderIcon("Assets/Textures/Metal_4.png", metalDevice),
  m_ReturnIcon("Assets/Textures/return_blue.png", metalDevice),
  m_FileImageIcon("Assets/Textures/image-file.png", metalDevice),
  m_ContentBrowserPanel()
{
    m_Icons[0] = ImTextureID(m_FolderIcon.GetTexture());
    m_Icons[1] = ImTextureID(m_FileIcon.GetTexture());
    m_Icons[2] = ImTextureID(m_ShaderIcon.GetTexture());
    m_Icons[3] = ImTextureID(m_ReturnIcon.GetTexture());
    m_Icons[4] = ImTextureID(m_FileImageIcon.GetTexture());
    
}

MacEditorLayer::~MacEditorLayer()
{
}

void MacEditorLayer::DrawContentBrowser()
{
    m_ContentBrowserPanel.Render(m_Icons);
}

