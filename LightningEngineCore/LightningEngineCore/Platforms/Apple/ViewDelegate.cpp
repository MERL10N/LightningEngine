//
//  ViewDelegate.cpp
//  LightningEngine
//
//  Created by Kian Marvi on 6/26/24.


#include "ViewDelegate.h"

ViewDelegate::ViewDelegate(MTK::View* view )
: MTK::ViewDelegate()
, renderer(MetalRenderer(view))
{
}

ViewDelegate::~ViewDelegate()
{
    renderer.Destroy();
}

void ViewDelegate::drawInMTKView( MTK::View *view )
{
    renderer.Draw(view);
}

