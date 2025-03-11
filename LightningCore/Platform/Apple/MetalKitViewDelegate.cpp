//
// Created by Kian Marvi on 11/24/24.
//

#include "MetalKitViewDelegate.h"
#include "../../Renderer/Metal/MetalRenderer.h"

MetalKitViewDelegate::MetalKitViewDelegate(MTL::Device *metalDevice)
    : ViewDelegate()
{
    metalRenderer = new MetalRenderer(metalDevice);
}

MetalKitViewDelegate::~MetalKitViewDelegate()
{
    if (metalRenderer)
        delete metalRenderer;
}

void MetalKitViewDelegate::drawInMTKView(MTK::View *metalKitView)
{
    metalRenderer->Render(metalKitView);
}
