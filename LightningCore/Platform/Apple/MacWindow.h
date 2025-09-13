//
// Created by Kian Marvi on 11/24/24.
//

#ifndef MACAPPLICATION_H
#define MACAPPLICATION_H

#include <AppKit/NSApplication.hpp>

namespace MTK
{
    class View;
}
namespace MTL
{
    class Device;
}

namespace NS
{
    class Window;
    class Menu;
    class MenuItem;
    class Application;
    class View;
}

class MetalKitViewDelegate;

class MacWindow : public NS::ApplicationDelegate
{

public:
    explicit MacWindow(unsigned int p_Width = 1920, unsigned int p_Height = 1080, const char* p_Title = "");
    void SetPreferredFramesPerSecond(float p_FPS);
    ~MacWindow();

    NS::Menu* createMenuBar();

    virtual void applicationWillFinishLaunching( NS::Notification* pNotification ) override;
    virtual void applicationDidFinishLaunching( NS::Notification* pNotification ) override;
    virtual bool applicationShouldTerminateAfterLastWindowClosed( NS::Application* pSender ) override;
    
    inline MTK::View* GetMetalKitView() { return m_MetalKitView; }
    inline MTL::Device* GetDevice() { return m_MetalDevice; }
    
    inline MetalKitViewDelegate* GetMTKViewDelegate() { return m_MetalKitViewDelegate; }
    
    inline unsigned int GetWidth() { return m_Width; }
    inline unsigned int GetHeight() { return m_Height; }

private:
    
    unsigned int m_Width, m_Height;
    const char* m_Title;
    
    MTK::View* m_MetalKitView;
    MTL::Device* m_MetalDevice;
    MetalKitViewDelegate* m_MetalKitViewDelegate = nullptr;
    
    NS::Window* m_Window;
    NS::Application* m_Application;
    NS::ApplicationDelegate* m_AppDelegate;
    
    NS::Menu* m_Menu;
    NS::Menu* m_MainMenu;
    NS::Menu* m_AppMenu;
    NS::Menu* m_WindowMenu;
    
    NS::String* m_AppName;
    NS::String* m_QuitItemName;
    
    NS::MenuItem* m_AppMenuItem;
    NS::MenuItem* pAppQuitItem;
    NS::MenuItem* m_WindowMenuItem;
    NS::MenuItem* m_CloseWindowItem;
    
    NS::View* m_View;
    CGRect frame;
 
};

#endif //MACAPPLICATION_H
