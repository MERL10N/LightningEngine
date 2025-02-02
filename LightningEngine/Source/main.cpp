#include "../../LightningCore/Platform/Apple/MacApplication.h"

int main()
{
    MacApplication macApp;

    NS::Application* sharedApplication = NS::Application::sharedApplication();

    sharedApplication->setDelegate(&macApp);
    sharedApplication->run();
    sharedApplication->release();
}
