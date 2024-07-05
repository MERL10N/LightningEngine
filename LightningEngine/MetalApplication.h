//
//  MyAppDelegate.h
//  LightningEngine
//
//  Created by Kian Marvi on 6/26/24.
//

#ifndef MyAppDelegate_h
#define MyAppDelegate_h
#include <AppKit/AppKit.hpp>

namespace MTK
{
    class View;
}

namespace MTL
{
    class Device;
}

class MyMTKViewDelegate;

class CMetalApplication : public NS::ApplicationDelegate
{
    public:
        ~CMetalApplication();

        NS::Menu* createMenuBar();

        virtual void applicationWillFinishLaunching( NS::Notification* pNotification ) override;
        virtual void applicationDidFinishLaunching( NS::Notification* pNotification ) override;
        virtual bool applicationShouldTerminateAfterLastWindowClosed( NS::Application* pSender ) override;

    private:
        NS::Window* appWindow;
        MTK::View* metalKitView;
        MTL::Device* metalDevice;
        MyMTKViewDelegate* viewDelegate = nullptr;
};

#endif /* MyAppDelegate_h */
