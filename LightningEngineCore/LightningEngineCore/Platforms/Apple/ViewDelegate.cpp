//
//  ViewDelegate.cpp
//  LightningEngine
//
//  Created by Kian Marvi on 6/26/24.


#include "ViewDelegate.h"

#include "MetalRenderer.h"

ViewDelegate::ViewDelegate( MTL::Device* metalDevice )
: MTK::ViewDelegate()
, metalRenderer( new MetalRenderer( metalDevice ) )
{
}

ViewDelegate::~ViewDelegate()
{
    delete metalRenderer;
}

void ViewDelegate::drawInMTKView( MTK::View* view )
{
    metalRenderer->Draw( view );
}

