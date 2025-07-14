#include <Platform/Apple/MacApplication.h>

int main()
{
    auto macApp = MacApplication(1920.f, 1080.f, "Lightning Game");
    macApp.Update();
}
