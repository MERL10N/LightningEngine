//
// Created by Kian Marvi on 11/24/24.
//

#ifndef MACAPPLICATION_H
#define MACAPPLICATION_H

#include <AppKit/AppKit.hpp>

namespace MTK
{
    class View;
}
namespace MTL
{
    class Device;
}

class MetalKitViewDelegate;

class MacApplication : public NS::ApplicationDelegate<MacApplication>
{
public:
    ~MacApplication();

    static NS::Menu* createMenuBar();

    void applicationWillFinishLaunching( NS::Notification* pNotification );
    void applicationDidFinishLaunching( NS::Notification* pNotification );
    bool applicationShouldTerminateAfterLastWindowClosed( NS::Application* pSender );

private:
    NS::Window* window;
    MTK::View* metalKitView;
    MTL::Device* metalDevice;
    MetalKitViewDelegate* metalKitViewDelegate = nullptr;
};

#endif //MACAPPLICATION_H
