//
// Created by Kian Marvi on 11/24/24.
//

#include "MetalKitViewDelegate.h"

MetalKitViewDelegate::MetalKitViewDelegate(MTL::Device *metalDevice)
    : ViewDelegate(),
      metalRenderer(MetalRenderer(metalDevice))
{
}

MetalKitViewDelegate::~MetalKitViewDelegate()
{
}

void MetalKitViewDelegate::drawInMTKView(MTK::View *metalKitView)
{
    metalRenderer.Render(metalKitView);
}
