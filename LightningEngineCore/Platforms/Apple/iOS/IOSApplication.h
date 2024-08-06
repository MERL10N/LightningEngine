//
//  IOSApplication.h
//  LightningEngine-iOS
//
//  Created by Kian Marvi on 8/3/24.
//

#ifndef IOSApplication_h
#define IOSApplication_h

#include <UIKit/UIKit.hpp>

namespace MTK
{
    class View;
}

namespace MTL
{
    class Device;
}

class ViewDelegate;

class IOSApplication : public UI::ApplicationDelegate
{
public:
        ~IOSApplication();

        bool applicationDidFinishLaunching( UI::Application *app, NS::Value *options ) override;
        void applicationWillTerminate( UI::Application *app ) override;

    private:
        UI::Window* window;
        UI::ViewController* viewController;
        MTK::View* metalKitView;
        MTL::Device* metalDevice;
        ViewDelegate* viewDelegate = nullptr;
};


#endif /* IOSApplication_h */
