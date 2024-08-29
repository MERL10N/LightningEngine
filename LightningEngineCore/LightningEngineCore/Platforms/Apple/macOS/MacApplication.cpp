/*
    MacApplication.cpp
    Description: This class is derived from the Apple's sample code "Learn Metal with C++"


    Copyright [2022] [Apple Inc]

    Licensed under the Apache License, Version 2.0 (the "License");
    you may not use this file except in compliance with the License.
    You may obtain a copy of the License at

     http://www.apache.org/licenses/LICENSE-2.0

    Unless required by applicable law or agreed to in writing, software
    distributed under the License is distributed on an "AS IS" BASIS,
    WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
    See the License for the specific language governing permissions and
    limitations under the License.
 
    Generated and Modified by Kian Marvi on 6/26/24.
*/

#include "MacApplication.h"
#include "ViewDelegate.h"
#include <Metal/Metal.hpp>
#include <Renderer/Metal/MetalDevice.h>

MTL::Device* MetalDevice::device = nullptr;

MacApplication::~MacApplication()
{
    #ifdef DEBUG
    editor.Destroy();
    #endif
    metalKitView->release();
    appWindow->release();
    
    MetalDevice::Cleanup();
    delete viewDelegate;
}

NS::Menu* MacApplication::createMenuBar()
{
    using NS::StringEncoding::UTF8StringEncoding;

    NS::Menu* mainMenu = NS::Menu::alloc()->init();
    NS::MenuItem* appMenuItem = NS::MenuItem::alloc()->init();
    NS::Menu* appMenu = NS::Menu::alloc()->init( NS::String::string( "Appname", UTF8StringEncoding ) );

    NS::String* appName = NS::RunningApplication::currentApplication()->localizedName();
    NS::String* quitItemName = NS::String::string( "Quit ", UTF8StringEncoding )->stringByAppendingString( appName );
    SEL quitCb = NS::MenuItem::registerActionCallback( "appQuit", [](void*,SEL,const NS::Object* sender){
        auto pApp = NS::Application::sharedApplication();
        pApp->terminate( sender );
    } );

    NS::MenuItem* appQuitItem = appMenu->addItem( quitItemName, quitCb, NS::String::string( "q", UTF8StringEncoding ) );
    appQuitItem->setKeyEquivalentModifierMask( NS::EventModifierFlagCommand );
    appMenuItem->setSubmenu( appMenu );

    NS::MenuItem* windowMenuItem = NS::MenuItem::alloc()->init();
    NS::Menu* windowMenu = NS::Menu::alloc()->init( NS::String::string( "Window", UTF8StringEncoding ) );

    SEL closeWindowCb = NS::MenuItem::registerActionCallback( "windowClose", [](void*, SEL, const NS::Object*){
        auto pApp = NS::Application::sharedApplication();
            pApp->windows()->object< NS::Window >(0)->close();
    } );
    NS::MenuItem* closeWindowItem = windowMenu->addItem( NS::String::string( "Close Window", UTF8StringEncoding ), closeWindowCb, NS::String::string( "w", UTF8StringEncoding ) );
    closeWindowItem->setKeyEquivalentModifierMask( NS::EventModifierFlagCommand );

    windowMenuItem->setSubmenu( windowMenu );

    mainMenu->addItem( appMenuItem );
    mainMenu->addItem( windowMenuItem );

    appMenuItem->release();
    windowMenuItem->release();
    appMenu->release();
    windowMenu->release();

    return mainMenu->autorelease();
}

void MacApplication::applicationWillFinishLaunching( NS::Notification* notification )
{
    NS::Menu* menu = createMenuBar();
    NS::Application* app = reinterpret_cast< NS::Application* >( notification->object() );
    app->setMainMenu( menu );
    app->setActivationPolicy( NS::ActivationPolicy::ActivationPolicyRegular );
}

void MacApplication::applicationDidFinishLaunching( NS::Notification* notification )
{
    CGRect frame = (CGRect){ {100.0, 100.0}, {1280.0, 720.0} };

    appWindow = NS::Window::alloc()->init(
        frame,
        NS::WindowStyleMaskClosable
        |NS::WindowStyleMaskTitled
        |NS::WindowStyleMaskResizable
        |NS::WindowStyleMaskMiniaturizable,
         NS::BackingStoreBuffered,
        false );
    
    MetalDevice::Init(); // Initialise metal device

    metalKitView = MTK::View::alloc()->init(frame, MetalDevice::GetDevice());
    
    
#ifdef DEBUG
    editor.Init(MetalDevice::GetDevice(), metalKitView);
#endif
    
   
    metalKitView->setColorPixelFormat( MTL::PixelFormat::PixelFormatBGRA8Unorm );
    metalKitView->setSampleCount(4);
    
    viewDelegate = new ViewDelegate(MetalDevice::GetDevice(), metalKitView);
    metalKitView->setDelegate( viewDelegate );
    
    appWindow->setContentView( metalKitView );
    appWindow->setTitle( NS::String::string( "Lightning Engine", NS::StringEncoding::UTF8StringEncoding ) );

    appWindow->makeKeyAndOrderFront( nullptr );

    NS::Application* pApp = reinterpret_cast< NS::Application* >(notification->object() );
    pApp->activateIgnoringOtherApps( true );
}

bool MacApplication::applicationShouldTerminateAfterLastWindowClosed( NS::Application* sender )
{
    return true;
}

