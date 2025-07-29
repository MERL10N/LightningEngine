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

#include <simd/simd.h>

class MetalFrameBuffer;


class MacEditor : public MTK::ViewDelegate
{
public:
    MacEditor(MTK::View* p_MetalKitView);
    virtual ~MacEditor() override;
    virtual void drawInMTKView(MTK::View* p_MetalKitView) override;

private:
    MetalRenderer m_MetalRenderer;
    MetalFrameBuffer m_MetalFrameBuffer;
    bool show_another_window;
    simd::float2 m_ViewportSize;
};


#endif /* MacEditor_hpp */
