//
//  view_delegate.h
//  metal_test
//
//  Created by Andrew Mengede on 13/9/2023.
//

#pragma once
#include "Scene2D.h"
class ViewDelegate : public MTK::ViewDelegate
{
    public:
        ViewDelegate(MTL::Device* device);
        virtual ~ViewDelegate() override;
        virtual void drawInMTKView(MTK::View* view) override;

    private:
//        Scene2D* renderer;
};
