//
// Created by Kian Marvi on 11/24/24.
//

#include "MetalKitViewDelegate.h"
#include "../../Renderer/Metal/MetalRenderer.h"

MetalKitViewDelegate::MetalKitViewDelegate(MTL::Device *metalDevice)
    : ViewDelegate()
{
    metalRenderer = new MetalRenderer;
    metalRenderer->Init(metalDevice);
}

MetalKitViewDelegate::~MetalKitViewDelegate()
{
    metalRenderer->Clean();
    if (metalRenderer != nullptr)
    {
        delete metalRenderer;
        metalRenderer = nullptr;
    }
}

void MetalKitViewDelegate::drawInMTKView(MTK::View *metalKitView)
{
    metalRenderer->Render(metalKitView);
}
