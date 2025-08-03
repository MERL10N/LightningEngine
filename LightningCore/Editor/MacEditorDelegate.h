//
//  MacEditorDelegate.hpp
//  LightningCore
//
//  Created by Kian Marvi on 7/13/25.
//

#ifndef MacEditorDelegate_hpp
#define MacEditorDelegate_hpp

#include <AppKit/AppKit.hpp>

namespace MTK
{
    class View;
}
namespace MTL
{
    class Device;
}

class MacEditor;

class MacEditorDelegate : public NS::ApplicationDelegate
{
public:
    //MacApplicationDelegate() = default;
    MacEditorDelegate(float p_Width = 1920.f, float p_Height = 1080.f, const char* p_Title = "");
    ~MacEditorDelegate();

    NS::Menu* createMenuBar();

    virtual void applicationWillFinishLaunching( NS::Notification* pNotification ) override;
    virtual void applicationDidFinishLaunching( NS::Notification* pNotification ) override;
    virtual bool applicationShouldTerminateAfterLastWindowClosed( NS::Application* pSender ) override;

private:
    
    float m_Width, m_Height;
    const char* m_Title;
    
    MTK::View* m_MetalKitView = nullptr;
    MTL::Device* m_MetalDevice = nullptr;
    MacEditor* m_MacEditor = nullptr;
    
    NS::Window* m_Window = nullptr;
    NS::Application* m_App = nullptr;
    
    NS::Menu* m_Menu = nullptr;
    NS::Menu* m_MainMenu = nullptr;
    NS::Menu* m_AppMenu = nullptr;
    NS::Menu* pWindowMenu = nullptr;
    
    NS::String* m_AppName = nullptr;
    NS::String* quitItemName = nullptr;
    
    NS::MenuItem* m_AppMenuItem = nullptr;
    NS::MenuItem* pAppQuitItem = nullptr;
    NS::MenuItem* pWindowMenuItem = nullptr;
    NS::MenuItem* pCloseWindowItem = nullptr;
    CGRect frame;
 
};

#endif /* MacEditorDelegate_hpp */
