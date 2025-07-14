//
// Created by Kian Marvi on 11/24/24.
//

#include "MacApplicationDelegate.h"
#include "MetalKitViewDelegate.h"
#include <Metal/Metal.hpp>
#include <MetalKit/MetalKit.hpp>

MacApplicationDelegate::MacApplicationDelegate(float p_Width, float p_Height, const char *p_Title)
: m_Width(p_Width),
  m_Height(p_Height),
  m_Title(p_Title),
  m_MetalDevice(MTL::CreateSystemDefaultDevice())
{
    frame = (CGRect){ {100.0, 100.0}, {static_cast<CGFloat>(m_Width), static_cast<CGFloat>(m_Height)} };
    m_MetalKitView = MTK::View::alloc()->init(frame, m_MetalDevice);
    m_MetalKitView->setColorPixelFormat(MTL::PixelFormatRGBA8Unorm);
    m_MetalKitView->setClearColor(MTL::ClearColor::Make(0.1f, 0.1f, 0.1f, 1.0 ));
    m_MetalKitView->setPreferredFramesPerSecond(60);
    
    m_MetalKitViewDelegate = new MetalKitViewDelegate(m_MetalKitView);
}



MacApplicationDelegate::~MacApplicationDelegate()
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

NS::Menu* MacApplicationDelegate::createMenuBar()
{
    using NS::StringEncoding::UTF8StringEncoding;

    pMainMenu = NS::Menu::alloc()->init();
    pAppMenuItem = NS::MenuItem::alloc()->init();
    pAppMenu = NS::Menu::alloc()->init( NS::String::string( "Appname", UTF8StringEncoding ) );

    appName = NS::RunningApplication::currentApplication()->localizedName();
    quitItemName = NS::String::string( "Quit ", UTF8StringEncoding )->stringByAppendingString( appName );
    SEL quitCb = NS::MenuItem::registerActionCallback( "appQuit", [](void*,SEL,const NS::Object* pSender){
        auto pApp = NS::Application::sharedApplication();
        pApp->terminate( pSender );
    } );

    pAppQuitItem = pAppMenu->addItem( quitItemName, quitCb, NS::String::string( "q", UTF8StringEncoding ) );
    pAppQuitItem->setKeyEquivalentModifierMask( NS::EventModifierFlagCommand );
    pAppMenuItem->setSubmenu( pAppMenu );

    pWindowMenuItem = NS::MenuItem::alloc()->init();
    pWindowMenu = NS::Menu::alloc()->init( NS::String::string( "Window", UTF8StringEncoding ) );

    SEL closeWindowCb = NS::MenuItem::registerActionCallback("windowClose", [](void *, SEL, const NS::Object *) {
        auto pApp = NS::Application::sharedApplication();
        pApp->windows()->object<NS::Window>(0)->close();
    });
    
    pCloseWindowItem = pWindowMenu->addItem( NS::String::string( "Close Window", UTF8StringEncoding ), closeWindowCb, NS::String::string( "w", UTF8StringEncoding));
    pCloseWindowItem->setKeyEquivalentModifierMask( NS::EventModifierFlagCommand );

    pWindowMenuItem->setSubmenu(pWindowMenu);

    pMainMenu->addItem(pAppMenuItem);
    pMainMenu->addItem(pWindowMenuItem);

    pAppMenuItem->release();
    pWindowMenuItem->release();
    pAppMenu->release();
    pWindowMenu->release();

    return pMainMenu->autorelease();
}

void MacApplicationDelegate::applicationWillFinishLaunching(NS::Notification* pNotification)
{
    m_Menu = createMenuBar();
    pApp = reinterpret_cast< NS::Application*>(pNotification->object());
    pApp->setMainMenu(m_Menu);
    pApp->setActivationPolicy(NS::ActivationPolicy::ActivationPolicyRegular);
}

void MacApplicationDelegate::applicationDidFinishLaunching(NS::Notification* pNotification)
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

    NS::Application* pApp = reinterpret_cast< NS::Application* >( pNotification->object() );
    pApp->activateIgnoringOtherApps( true );
}

bool MacApplicationDelegate::applicationShouldTerminateAfterLastWindowClosed( NS::Application* pSender )
{
    return true;
}

