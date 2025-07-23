//
// Created by Kian Marvi on 11/24/24.
//

#include "MetalKitViewDelegate.h"

MetalKitViewDelegate::MetalKitViewDelegate(MTK::View* p_MetalKitView)
    : ViewDelegate(),
      metalRenderer(MetalRenderer(p_MetalKitView->device()))
{
}

MetalKitViewDelegate::~MetalKitViewDelegate()
{
}

void MetalKitViewDelegate::drawInMTKView(MTK::View *p_MetalKitView)
{
    metalRenderer.BeginFrame();
    metalRenderer.Render(p_MetalKitView);
    metalRenderer.Commit(true);
}
