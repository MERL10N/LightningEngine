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

class MacApplicationDelegate : public NS::ApplicationDelegate
{
public:
    ~MacApplicationDelegate();

    static NS::Menu* createMenuBar();

    virtual void applicationWillFinishLaunching( NS::Notification* pNotification ) override;
    virtual void applicationDidFinishLaunching( NS::Notification* pNotification ) override;
    virtual bool applicationShouldTerminateAfterLastWindowClosed( NS::Application* pSender ) override;

private:
    NS::Window* window;
    MTK::View* metalKitView;
    MTL::Device* metalDevice;
    MetalKitViewDelegate* metalKitViewDelegate = nullptr;
};

#endif //MACAPPLICATION_H
