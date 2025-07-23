//
//  MacApplication.hpp
//  LightningCore
//
//  Created by Kian Marvi on 7/3/25.
//

#ifndef MacApplication_hpp
#define MacApplication_hpp

#include "MacApplicationDelegate.h"
#include "../Window.h"


class MacApplication : public Window
{
public:
    MacApplication(float p_Width = 1920.f, float p_Height = 1080.f, const char* p_Title = "Lightning Game");
    ~MacApplication();
    void Update();

private:
    NS::Application* m_SharedApplication;
    MacApplicationDelegate macAppDelegate;
};

#endif /* MacApplication_hpp */
