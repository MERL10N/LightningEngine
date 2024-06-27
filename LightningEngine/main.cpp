//  main.cpp
//  Metal Engine

#ifdef __APPLE__
//#include "MetalApplication.h"
#include "MetalCPPApplication.h"
#else
#include "Application.h"
#endif
#include <iostream>

int main(void)
{

#ifdef __APPLE__
    
    CMetalCPPApplication del;

    NS::Application* pApp = NS::Application::sharedApplication();
    pApp->setDelegate( &del );
    pApp->run();


#else

    
        // if the application is initialised properly, then run it
        if (pApp->Init() == true)
        {
            // Run the Application instance
            pApp->Run();

            // Destroy the Application instance, just to be sure
            pApp->Destroy();

            // Set the Application handler to nullptr
            pApp = nullptr;

            // Return 0 since the application successfully ran
            return 0;
        }

        // Return 1 if the application failed to run
        return 1;
     
    
    CApplication* pApp = CApplication::GetInstance();
#endif
    
    
}
