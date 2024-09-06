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
    
    MacApplication del;

    NS::Application* pApp = NS::Application::sharedApplication();
    pApp->setDelegate( &del );
    pApp->run();
    pApp->release();

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
