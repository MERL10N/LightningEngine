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
    //MacApplicationDelegate() = default;
    MacApplicationDelegate(float p_Width = 1920.f, float p_Height = 1080.f, const char* p_Title = "");
    ~MacApplicationDelegate();

    NS::Menu* createMenuBar();

    virtual void applicationWillFinishLaunching( NS::Notification* pNotification ) override;
    virtual void applicationDidFinishLaunching( NS::Notification* pNotification ) override;
    virtual bool applicationShouldTerminateAfterLastWindowClosed( NS::Application* pSender ) override;
    
    inline MTK::View* GetMetalKitView() { return m_MetalKitView; }
    inline MTL::Device* GetDevice() { return m_MetalDevice; }
    
    inline MetalKitViewDelegate* GetMTKViewDelegate() { return m_MetalKitViewDelegate; }

private:
    
    float m_Width, m_Height;
    const char* m_Title;
    
    MTK::View* m_MetalKitView;
    MTL::Device* m_MetalDevice;
    MetalKitViewDelegate* m_MetalKitViewDelegate = nullptr;
    
    NS::Window* m_Window;
    NS::Application* pApp;
    
    NS::Menu* m_Menu;
    NS::Menu* pMainMenu;
    NS::Menu* pAppMenu;
    NS::Menu* pWindowMenu;
    
    NS::String* appName;
    NS::String* quitItemName;
    
    NS::MenuItem* pAppMenuItem;
    NS::MenuItem* pAppQuitItem;
    NS::MenuItem* pWindowMenuItem;
    NS::MenuItem* pCloseWindowItem;
    CGRect frame;
 
};

#endif //MACAPPLICATION_H
