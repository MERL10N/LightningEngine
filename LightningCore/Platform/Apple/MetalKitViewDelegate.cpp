//
// Created by Kian Marvi on 11/24/24.
//

#include "MetalKitViewDelegate.h"

MetalKitViewDelegate::MetalKitViewDelegate(MTK::View* p_MetalKitView)
    : ViewDelegate(),
      m_MetalRenderer(MetalRenderer(p_MetalKitView->device()))
{
}

MetalKitViewDelegate::~MetalKitViewDelegate()
{
}

void MetalKitViewDelegate::drawInMTKView(MTK::View *p_MetalKitView)
{
    m_MetalRenderer.BeginFrame();
    m_MetalRenderer.Render(p_MetalKitView);
    m_MetalRenderer.Commit(true);
}
