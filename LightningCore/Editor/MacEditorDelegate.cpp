//
//  MacEditorDelegate.cpp
//  LightningCore
//
//  Created by Kian Marvi on 7/13/25.
//

#include "MacEditorDelegate.hpp"

#include "MacEditor.h"
#include "MetalKitViewDelegate.h"
#include <Metal/Metal.hpp>
#include <MetalKit/MetalKit.hpp>


MacEditorDelegate::MacEditorDelegate(float p_Width, float p_Height, const char *p_Title)
: m_Width(p_Width),
  m_Height(p_Height),
  m_Title(p_Title),
  m_MetalDevice(MTL::CreateSystemDefaultDevice())
{
    m_Frame = (CGRect){ {100.0, 100.0}, {static_cast<CGFloat>(m_Width), static_cast<CGFloat>(m_Height)} };
    m_MetalKitView = MTK::View::alloc()->init(m_Frame, m_MetalDevice);
    m_MetalKitView->setColorPixelFormat(MTL::PixelFormatRGBA8Unorm);
    m_MetalKitView->setClearColor(MTL::ClearColor::Make(0.0, 0.0, 0.0, 0.0));
    m_MetalKitView->setPreferredFramesPerSecond(120);
}



MacEditorDelegate::~MacEditorDelegate()
{
    if (m_MetalKitView)
    {
        m_MetalKitView->release();
        m_MetalKitView = nullptr;
    }
   
    if (m_Window)
    {
        m_Window->release();
        m_Window = nullptr;
    }
    
    if (m_MetalDevice)
    {
        m_MetalDevice->release();
        m_MetalDevice = nullptr;
    }
    
    if (m_MacEditor)
    {
        delete m_MacEditor;
        m_MacEditor = nullptr;
    }
}

NS::Menu* MacEditorDelegate::createMenuBar()
{
    using NS::StringEncoding::UTF8StringEncoding;

    m_MainMenu = NS::Menu::alloc()->init();
    m_AppMenuItem = NS::MenuItem::alloc()->init();
    m_AppMenu = NS::Menu::alloc()->init(NS::String::string("Appname", UTF8StringEncoding));

    m_AppName = NS::RunningApplication::currentApplication()->localizedName();
    quitItemName = NS::String::string( "Quit ", UTF8StringEncoding )->stringByAppendingString( m_AppName );
    
    SEL quitCb = NS::MenuItem::registerActionCallback( "appQuit", [](void*,SEL,const NS::Object* pSender){
        auto pApp = NS::Application::sharedApplication();
        pApp->terminate( pSender );
    } );

    m_AppQuitItem = m_AppMenu->addItem( quitItemName, quitCb, NS::String::string( "q", UTF8StringEncoding ) );
    m_AppQuitItem->setKeyEquivalentModifierMask( NS::EventModifierFlagCommand );
    m_AppMenuItem->setSubmenu( m_AppMenu );

    m_WindowMenuItem = NS::MenuItem::alloc()->init();
    pWindowMenu = NS::Menu::alloc()->init( NS::String::string( "Window", UTF8StringEncoding ) );

    SEL closeWindowCb = NS::MenuItem::registerActionCallback("windowClose", [](void *, SEL, const NS::Object *) {
        auto pApp = NS::Application::sharedApplication();
        pApp->windows()->object<NS::Window>(0)->close();
    });
    
    pCloseWindowItem = pWindowMenu->addItem( NS::String::string( "Close Window", UTF8StringEncoding ), closeWindowCb, NS::String::string( "w", UTF8StringEncoding));
    pCloseWindowItem->setKeyEquivalentModifierMask( NS::EventModifierFlagCommand );

    m_WindowMenuItem->setSubmenu(pWindowMenu);

    m_MainMenu->addItem(m_AppMenuItem);
    m_MainMenu->addItem(m_WindowMenuItem);

    m_AppMenuItem->release();
    m_WindowMenuItem->release();
    m_AppMenu->release();
    pWindowMenu->release();

    return m_MainMenu->autorelease();
}

void MacEditorDelegate::applicationWillFinishLaunching(NS::Notification* p_Notification)
{
    m_Menu = createMenuBar();
    m_App = reinterpret_cast< NS::Application*>(p_Notification->object());
    m_App->setMainMenu(m_Menu);
    m_App->setActivationPolicy(NS::ActivationPolicy::ActivationPolicyRegular);
}

void MacEditorDelegate::applicationDidFinishLaunching(NS::Notification* p_Notification)
{
    m_Window = NS::Window::alloc()->init(
         m_Frame,
         NS::WindowStyleMaskClosable
        |NS::WindowStyleMaskTitled
        |NS::WindowStyleMaskResizable,
         NS::BackingStoreBuffered, false);


    m_MacEditor = new MacEditor(m_MetalKitView);
    m_MetalKitView->setDelegate(m_MacEditor);

    m_Window->setContentView(m_MetalKitView);
    m_Window->setTitle(NS::String::string(m_Title, NS::StringEncoding::UTF8StringEncoding));
    m_Window->makeKeyAndOrderFront(nullptr);

    m_App = reinterpret_cast<NS::Application*>(p_Notification->object());
    m_App->activateIgnoringOtherApps(true);
}

bool MacEditorDelegate::applicationShouldTerminateAfterLastWindowClosed( NS::Application* pSender )
{
    return true;
}
