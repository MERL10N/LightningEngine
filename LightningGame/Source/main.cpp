#include <Platform/Apple/MacApplication.h>

int main()
{
    auto macApp = MacApplication();
    macApp.Update(60.f);
}
