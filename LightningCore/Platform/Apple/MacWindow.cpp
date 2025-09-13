//
// Created by Kian Marvi on 11/24/24.
//

#include "MacWindow.h"
#include "MetalKitViewDelegate.h"
#include <Metal/Metal.hpp>
#include <MetalKit/MetalKit.hpp>


MacWindow::MacWindow(unsigned int p_Width, unsigned int p_Height, const char *p_Title)
: m_Width(p_Width),
  m_Height(p_Height),
  m_Title(p_Title),
  m_MetalDevice(MTL::CreateSystemDefaultDevice()),
  m_Application(NS::Application::sharedApplication())
{
    frame = (CGRect){ {100.0, 100.0}, {static_cast<CGFloat>(m_Width), static_cast<CGFloat>(m_Height)} };
    m_MetalKitView = MTK::View::alloc()->init(frame, m_MetalDevice);
    m_MetalKitView->setColorPixelFormat(MTL::PixelFormatRGBA8Unorm);
    m_MetalKitView->setClearColor(MTL::ClearColor::Make(0.1f, 0.1f, 0.1f, 1.0 ));
    m_MetalKitViewDelegate = new MetalKitViewDelegate(m_MetalKitView);
}

void MacWindow::SetPreferredFramesPerSecond(float p_FPS)
{
    m_MetalKitView->setPreferredFramesPerSecond(p_FPS);
}


MacWindow::~MacWindow()
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
    
    if (m_MetalKitViewDelegate)
    {
        delete m_MetalKitViewDelegate;
        m_MetalKitViewDelegate = nullptr;
    }
}

NS::Menu* MacWindow::createMenuBar()
{
    using NS::StringEncoding::UTF8StringEncoding;

    m_MainMenu = NS::Menu::alloc()->init();
    m_AppMenuItem = NS::MenuItem::alloc()->init();
    m_AppMenu = NS::Menu::alloc()->init( NS::String::string( "Appname", UTF8StringEncoding ) );

    m_AppName = NS::RunningApplication::currentApplication()->localizedName();
    m_QuitItemName = NS::String::string( "Quit ", UTF8StringEncoding )->stringByAppendingString( m_AppName );
    
    SEL quitCb = NS::MenuItem::registerActionCallback( "appQuit", [](void*,SEL,const NS::Object* pSender){
        auto pApp = NS::Application::sharedApplication();
        pApp->terminate( pSender );
    } );

    pAppQuitItem = m_AppMenu->addItem( m_QuitItemName, quitCb, NS::String::string( "q", UTF8StringEncoding ) );
    pAppQuitItem->setKeyEquivalentModifierMask( NS::EventModifierFlagCommand );
    m_AppMenuItem->setSubmenu( m_AppMenu );

    m_WindowMenuItem = NS::MenuItem::alloc()->init();
    m_WindowMenu = NS::Menu::alloc()->init( NS::String::string( "Window", UTF8StringEncoding ) );

    SEL closeWindowCb = NS::MenuItem::registerActionCallback("windowClose", [](void *, SEL, const NS::Object *) {
        auto pApp = NS::Application::sharedApplication();
        pApp->windows()->object<NS::Window>(0)->close();
    });
    
    m_CloseWindowItem = m_WindowMenu->addItem( NS::String::string( "Close Window", UTF8StringEncoding ), closeWindowCb, NS::String::string( "w", UTF8StringEncoding));
    m_CloseWindowItem->setKeyEquivalentModifierMask( NS::EventModifierFlagCommand );

    m_WindowMenuItem->setSubmenu(m_WindowMenu);

    m_MainMenu->addItem(m_AppMenuItem);
    m_MainMenu->addItem(m_WindowMenuItem);

    m_AppMenuItem->release();
    m_WindowMenuItem->release();
    m_AppMenu->release();
    m_WindowMenu->release();

    return m_MainMenu->autorelease();
}

void MacWindow::applicationWillFinishLaunching(NS::Notification* pNotification)
{
    m_Menu = createMenuBar();
    m_Application = reinterpret_cast< NS::Application*>(pNotification->object());
    m_Application->setMainMenu(m_Menu);
    m_Application->setActivationPolicy(NS::ActivationPolicy::ActivationPolicyRegular);
}

void MacWindow::applicationDidFinishLaunching(NS::Notification* pNotification)
{
    m_Window = NS::Window::alloc()->init(
        frame,
        NS::WindowStyleMaskClosable
        |NS::WindowStyleMaskTitled
        |NS::WindowStyleMaskResizable,
        NS::BackingStoreBuffered, false);

   
    m_MetalKitView->setDelegate( m_MetalKitViewDelegate );

    m_Window->setContentView( m_MetalKitView );
    m_Window->setTitle(NS::String::string(m_Title, NS::StringEncoding::UTF8StringEncoding));
    m_Window->makeKeyAndOrderFront(nullptr);

    NS::Application* p_App = reinterpret_cast< NS::Application* >( pNotification->object() );
    p_App->activateIgnoringOtherApps( true );
}

bool MacWindow::applicationShouldTerminateAfterLastWindowClosed( NS::Application* pSender )
{
    return true;
}



