//
//  MacApplication.hpp
//  LightningCore
//
//  Created by Kian Marvi on 7/3/25.
//

#ifndef MacApplication_hpp
#define MacApplication_hpp

#include "MacApplicationDelegate.h"


class MacApplication
{
public:
    MacApplication();
    ~MacApplication();
    void Update();

private:
    NS::Application* m_SharedApplication;
    MacApplicationDelegate macAppDelegate;
};

#endif /* MacApplication_hpp */
