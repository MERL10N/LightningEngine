//
//  main.cpp
//  LightningEditor
//
//  Created by Kian Marvi on 3/8/25.
//

#include <Platform/Apple/MacApplication.h>

int main()
{
    MacApplication macApp;

    NS::Application* sharedApplication = NS::Application::sharedApplication();

    sharedApplication->setDelegate(&macApp);
    sharedApplication->run();
    sharedApplication->release();
}
