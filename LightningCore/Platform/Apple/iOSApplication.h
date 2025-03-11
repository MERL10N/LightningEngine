//
//  iOSApplication.h
//  LightningCore
//
//  Created by Kian Marvi on 3/5/25.
//
#include <UIKit/UIKit.hpp>
class MetalKitViewDelegate;

namespace MTK
{
    class View;
}
namespace MTL
{
    class Device;
}

class iOSApplication : public UI::ApplicationDelegate
{
public:
    ~iOSApplication();
    
    virtual bool applicationDidFinishLaunching( UI::Application *pApp, NS::Value *options ) override;
    virtual void applicationWillTerminate( UI::Application *pApp ) override;
    
private:
    UI::Window* window;
    UI::ViewController* viewController;
    MTK::View* metalKitView;
    MTL::Device* _pDevice;
    MetalKitViewDelegate* viewDelegate = nullptr;
};

