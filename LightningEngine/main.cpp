//  main.cpp
//  Metal Engine

#ifdef __APPLE__
#include "MetalApplication.h"
#else
#include "Application.h"
#endif
#include <iostream>

int main()
{

#ifdef __APPLE__
        CMetalApplication* pApp = CMetalApplication::GetInstance();
#else
     CApplication* pApp = CApplication::GetInstance();
#endif
        // if the application is initialised properly, then run it
        if (pApp->init() == true)
        {
            // Run the Application instance
            pApp->Run();

            // Destroy the Application instance, just to be sure
            pApp->Destroy();

            // Set the Application handler to NULL
            pApp = nullptr;

            // Return 0 since the application successfully ran
            return 0;
        }

        // Return 1 if the application failed to run
        return 1;
    
}
