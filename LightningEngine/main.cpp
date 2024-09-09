//  main.cpp
//  Lightning Engine
#ifdef __APPLE__
    #include "Source/Platforms/Apple/macOS/MacApplication.h"
#else
    #include "Application.h"
#endif

int main()
{

#ifdef __APPLE__
    
    MacApplication macApp;
    NS::Application* app = NS::Application::sharedApplication();
    app->setDelegate(&macApp);
    app->run();
    app->release();

#else
    /*
    Application app;
    app.init();
    app.run();
    app.release();
     */
#endif
    
    return 0;
}
