//
//  MyMTKViewDelegate.h
//  LightningEngine
//
//  Created by Kian Marvi on 6/26/24.
//

#ifndef MyMTKViewDelegate_h
#define MyMTKViewDelegate_h

#include <MetalKit/MetalKit.hpp>

namespace MTL
{
class Device;
}

class CRenderer;

class MyMTKViewDelegate : public MTK::ViewDelegate
{
    public:
        MyMTKViewDelegate( MTL::Device* pDevice );
        virtual ~MyMTKViewDelegate() override;
        virtual void drawInMTKView( MTK::View* pView ) override;

    private:
        CRenderer* _pRenderer;
};

#endif /* MyMTKViewDelegate_h */
