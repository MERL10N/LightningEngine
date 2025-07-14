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
    
    MTK::View* m_MetalKitView;
    MTL::Device* m_MetalDevice;
    MacEditor* m_MacEditor = nullptr;
    
    NS::Window* m_Window;
    NS::Application* m_App;
    
    NS::Menu* m_Menu;
    NS::Menu* m_MainMenu;
    NS::Menu* m_AppMenu;
    NS::Menu* pWindowMenu;
    
    NS::String* m_AppName;
    NS::String* quitItemName;
    
    NS::MenuItem* m_AppMenuItem;
    NS::MenuItem* pAppQuitItem;
    NS::MenuItem* pWindowMenuItem;
    NS::MenuItem* pCloseWindowItem;
    CGRect frame;
 
};

#endif /* MacEditorDelegate_hpp */
