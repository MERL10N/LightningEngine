//
//  main.cpp
//  LightningEditor
//
//  Created by Kian Marvi on 3/8/25.
//

#include "Editor/Mac/MacEditorApplication.h"
#include <print>

int main()
{
#ifdef __APPLE__
    auto pEditorApp = MacEditorApplication();
    pEditorApp.Update();
#else
    std::println("Lightning Engine not supported on other platforms");
#endif
    
}
