//
// Created by Kian Marvi on 11/24/24.
//

#include "MacApplication.h"
#include "MetalKitViewDelegate.h"
#include <Metal/Metal.hpp>
#include <MetalKit/MetalKit.hpp>

MacApplication::~MacApplication()
{
    if (metalKitView)
    {
        metalKitView->release();
        metalKitView = nullptr;
    }
   
    if (window)
    {
        window->release();
        window = nullptr;
    }
    
    if (metalDevice)
    {
        metalDevice->release();
        metalDevice = nullptr;
    }
    
    if (metalKitViewDelegate)
    {
        delete metalKitViewDelegate;
        metalKitViewDelegate = nullptr;
    }
}

NS::Menu* MacApplication::createMenuBar()
{
    using NS::StringEncoding::UTF8StringEncoding;

    NS::Menu* pMainMenu = NS::Menu::alloc()->init();
    NS::MenuItem* pAppMenuItem = NS::MenuItem::alloc()->init();
    NS::Menu* pAppMenu = NS::Menu::alloc()->init( NS::String::string( "Appname", UTF8StringEncoding ) );

    NS::String* appName = NS::RunningApplication::currentApplication()->localizedName();
    NS::String* quitItemName = NS::String::string( "Quit ", UTF8StringEncoding )->stringByAppendingString( appName );
    SEL quitCb = NS::MenuItem::registerActionCallback( "appQuit", [](void*,SEL,const NS::Object* pSender){
        auto pApp = NS::Application::sharedApplication();
        pApp->terminate( pSender );
    } );

    NS::MenuItem* pAppQuitItem = pAppMenu->addItem( quitItemName, quitCb, NS::String::string( "q", UTF8StringEncoding ) );
    pAppQuitItem->setKeyEquivalentModifierMask( NS::EventModifierFlagCommand );
    pAppMenuItem->setSubmenu( pAppMenu );

    NS::MenuItem* pWindowMenuItem = NS::MenuItem::alloc()->init();
    NS::Menu* pWindowMenu = NS::Menu::alloc()->init( NS::String::string( "Window", UTF8StringEncoding ) );

    SEL closeWindowCb = NS::MenuItem::registerActionCallback("windowClose", [](void *, SEL, const NS::Object *) {
        auto pApp = NS::Application::sharedApplication();
        pApp->windows()->object<NS::Window>(0)->close();
    });
    NS::MenuItem* pCloseWindowItem = pWindowMenu->addItem( NS::String::string( "Close Window", UTF8StringEncoding ), closeWindowCb, NS::String::string( "w", UTF8StringEncoding));
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

void MacApplication::applicationWillFinishLaunching(NS::Notification* pNotification)
{
    NS::Menu* pMenu = createMenuBar();
    NS::Application* pApp = reinterpret_cast< NS::Application*>(pNotification->object());
    pApp->setMainMenu(pMenu);
    pApp->setActivationPolicy(NS::ActivationPolicy::ActivationPolicyRegular);
}

void MacApplication::applicationDidFinishLaunching(NS::Notification* pNotification)
{
    auto frame = (CGRect){ {100.0, 100.0}, {1280.0, 720.0} };

    window = NS::Window::alloc()->init(
        frame,
        NS::WindowStyleMaskClosable
        |NS::WindowStyleMaskTitled
        |NS::WindowStyleMaskResizable,
        NS::BackingStoreBuffered, false);

    metalDevice = MTL::CreateSystemDefaultDevice();

    metalKitView = MTK::View::alloc()->init(frame, metalDevice);
    metalKitView->setColorPixelFormat(MTL::PixelFormatBGRA8Unorm);
    metalKitView->setClearColor(MTL::ClearColor::Make(0.1f, 0.1f, 0.1f, 1.0 ));
    metalKitView->setPreferredFramesPerSecond(120);

    metalKitViewDelegate = new MetalKitViewDelegate(metalDevice);
    metalKitView->setDelegate( metalKitViewDelegate );

    window->setContentView( metalKitView );
    window->setTitle( NS::String::string("Lightning Engine (Reborn)", NS::StringEncoding::UTF8StringEncoding));
    window->makeKeyAndOrderFront(nullptr);

    NS::Application* pApp = reinterpret_cast< NS::Application* >( pNotification->object() );
    pApp->activateIgnoringOtherApps( true );
}

bool MacApplication::applicationShouldTerminateAfterLastWindowClosed( NS::Application* pSender )
{
    return true;
}
