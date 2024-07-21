//
//  MyAppDelegate.cpp
//  LightningEngine
//
//  Created by Kian Marvi on 6/26/24.

#include "MetalApplication.h"
#include "MyMTKViewDelegate.h"
#include <Editor/Editor.h>
#include <Metal/Metal.hpp>

CMetalApplication::~CMetalApplication()
{
    CEditor::GetInstance()->Destroy();
    metalKitView->release();
    appWindow->release();
    metalDevice->release();
    delete viewDelegate;
}

NS::Menu* CMetalApplication::createMenuBar()
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

    SEL closeWindowCb = NS::MenuItem::registerActionCallback( "windowClose", [](void*, SEL, const NS::Object*){
        auto pApp = NS::Application::sharedApplication();
            pApp->windows()->object< NS::Window >(0)->close();
    } );
    NS::MenuItem* pCloseWindowItem = pWindowMenu->addItem( NS::String::string( "Close Window", UTF8StringEncoding ), closeWindowCb, NS::String::string( "w", UTF8StringEncoding ) );
    pCloseWindowItem->setKeyEquivalentModifierMask( NS::EventModifierFlagCommand );

    pWindowMenuItem->setSubmenu( pWindowMenu );

    pMainMenu->addItem( pAppMenuItem );
    pMainMenu->addItem( pWindowMenuItem );

    pAppMenuItem->release();
    pWindowMenuItem->release();
    pAppMenu->release();
    pWindowMenu->release();

    return pMainMenu->autorelease();
}

void CMetalApplication::applicationWillFinishLaunching( NS::Notification* pNotification )
{
    NS::Menu* pMenu = createMenuBar();
    NS::Application* pApp = reinterpret_cast< NS::Application* >( pNotification->object() );
    pApp->setMainMenu( pMenu );
    pApp->setActivationPolicy( NS::ActivationPolicy::ActivationPolicyRegular );
}

void CMetalApplication::applicationDidFinishLaunching( NS::Notification* pNotification )
{
    CGRect frame = (CGRect){ {100.0, 100.0}, {1280.0, 720.0} };

    appWindow = NS::Window::alloc()->init(
        frame,
        NS::WindowStyleMaskClosable|NS::WindowStyleMaskTitled|NS::WindowStyleMaskResizable|NS::WindowStyleMaskMiniaturizable,
        NS::BackingStoreBuffered,
        false );
    
    metalDevice = MTL::CreateSystemDefaultDevice();

    metalKitView = MTK::View::alloc()->init( frame, metalDevice );
    CEditor::GetInstance()->Init(metalDevice, metalKitView);
    
    metalKitView->setPreferredFramesPerSecond(120);
    metalKitView->setColorPixelFormat( MTL::PixelFormat::PixelFormatBGRA8Unorm );
    
    viewDelegate = new MyMTKViewDelegate( metalDevice );
    metalKitView->setDelegate( viewDelegate );
    
    appWindow->setContentView( metalKitView );
    appWindow->setTitle( NS::String::string( "Lightning Engine", NS::StringEncoding::UTF8StringEncoding ) );

    appWindow->makeKeyAndOrderFront( nullptr );

    NS::Application* pApp = reinterpret_cast< NS::Application* >( pNotification->object() );
    pApp->activateIgnoringOtherApps( true );
}

bool CMetalApplication::applicationShouldTerminateAfterLastWindowClosed( NS::Application* pSender )
{
    return true;
}
