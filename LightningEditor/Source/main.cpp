//
//  main.cpp
//  LightningEditor
//
//  Created by Kian Marvi on 3/8/25.
//

#ifdef __APPLE__
#include "Editor/Mac/MacEditorApplication.h"
#endif
#include <print>

int main()
{
#ifdef __APPLE__
    auto editor = MacEditorApplication();
    editor.Update();
#else
    std::println("Lightning Engine not supported on other platforms");
#endif
    
}
