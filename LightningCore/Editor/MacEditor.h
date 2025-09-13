//
//  MacEditor.h
//  LightningCore
//
//  Created by Kian Marvi on 7/9/25.
//

#ifndef MACEDITOR_H
#define MACEDITOR_H

namespace MTK
{
    class View;
}

#include <MetalKit/MetalKit.hpp>
#include "../Renderer/Metal/MetalRenderer.h"
#include "../Renderer/Metal/MetalFrameBuffer.h"
#include "EditorLayer.h"
#include <simd/simd.h>

class MetalFrameBuffer;


struct ImFont;
struct ImTextureRef;

class MacEditor : public MTK::ViewDelegate
{
public:
    explicit MacEditor(MTK::View* p_MetalKitView);
    virtual ~MacEditor() final;
    
    void DrawGameViewport();
    
    void drawInMTKView(MTK::View* p_MetalKitView) final;

private:
    MetalRenderer m_MetalRenderer;
    MetalFrameBuffer m_MetalFrameBuffer;
    simd::float2 m_ViewportSize;
    
    ImTextureRef* m_PlayButton = nullptr;
    ImFont* m_Font = nullptr;
    
    EditorLayer m_EditorLayer;
    
    float m_AspectRatio, m_FontScaleFactor;
};


#endif /* MacEditor_hpp */
