//
// Created by Kian Marvi on 11/24/24.
//

#include "MetalKitViewDelegate.h"

MetalKitViewDelegate::MetalKitViewDelegate(MTL::Device *metalDevice)
    : ViewDelegate(),
      metalRenderer(metalDevice)
{
}

MetalKitViewDelegate::~MetalKitViewDelegate()
{
    metalRenderer.Clean();
}

void MetalKitViewDelegate::drawInMTKView(MTK::View *metalKitView)
{
    metalRenderer.Draw(metalKitView);
}
