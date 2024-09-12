//
//  ViewDelegate.cpp
//  LightningEngine
//
//  Created by Kian Marvi on 6/26/24.


#include "ViewDelegate.h"
#include <LightningEngineCore/MetalRenderer.h>

ViewDelegate::ViewDelegate(MTK::View* view)
: MTK::ViewDelegate()
, renderer(new MetalRenderer(view))
{
}

ViewDelegate::~ViewDelegate()
{
    delete renderer;
}

void ViewDelegate::drawInMTKView(MTK::View *view)
{
    renderer->Draw(view);
}

