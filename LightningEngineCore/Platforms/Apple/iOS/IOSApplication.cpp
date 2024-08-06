//
//  IOSApplication.cpp
//  LightningEngine-iOS
//
//  Created by Kian Marvi on 8/3/24.
//

#include "IOSApplication.h"
#include "../../ViewDelegate.h"
//#include <Editor/Editor.h>
#include <Metal/Metal.hpp>


IOSApplication::~IOSApplication()
{
   // CEditor::GetInstance()->Destroy();
    viewController->release();
    metalKitView->release();
    window->release();
    metalDevice->release();
    delete viewDelegate;
}




bool IOSApplication::applicationDidFinishLaunching(  UI::Application *app, NS::Value *options )
{
    CGRect frame = UI::Screen::mainScreen()->bounds();

       window = UI::Window::alloc()->init(frame);

        viewController = UI::ViewController::alloc()->init( nullptr, nullptr );

       metalDevice = MTL::CreateSystemDefaultDevice();

       metalKitView = MTK::View::alloc()->init( frame, metalDevice );
       //CEditor::GetInstance()->Init(metalDevice, metalKitView);
        metalKitView->setColorPixelFormat( MTL::PixelFormat::PixelFormatBGRA8Unorm_sRGB );
        metalKitView->setClearColor( MTL::ClearColor::Make( 1.0, 0.0, 0.0, 1.0 ) );

       viewDelegate = new ViewDelegate( metalDevice );
        metalKitView->setDelegate( viewDelegate );

       UI::View *mtkView = (UI::View *)metalKitView;
       mtkView->setAutoresizingMask( UI::ViewAutoresizingFlexibleWidth | UI::ViewAutoresizingFlexibleHeight );
       viewController->view()->addSubview( mtkView );
       window->setRootViewController( viewController );

       window->makeKeyAndVisible();

       return true;
}

void IOSApplication::applicationWillTerminate( UI::Application* app )
{
}
