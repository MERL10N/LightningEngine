//
//  main.cpp
//  LightningEditor
//
//  Created by Kian Marvi on 3/8/25.
//

#include "MacEditorApplication.h"

int main()
{
    auto pEditorApp = MacEditorApplication(1920, 1080, "Lightning Editor");
    pEditorApp.Update();
    
}
