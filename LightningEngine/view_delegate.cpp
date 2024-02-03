//
//  view_delegate.cpp
//  metal_test
//
//  Created by Andrew Mengede on 13/9/2023.
//

#include "view_delegate.h"

ViewDelegate::ViewDelegate(MTL::Device* device)
: MTK::ViewDelegate()
{
//    renderer = Scene2D::GetInstance();
//    renderer->init(device);
    Scene2D::GetInstance()->init(device);
}

ViewDelegate::~ViewDelegate()
{
//    renderer->Destroy();
    Scene2D::GetInstance()->Destroy();
}

void ViewDelegate::drawInMTKView(MTK::View* view)
{
//    renderer->draw(view);
    Scene2D::GetInstance()->draw(view);
}
