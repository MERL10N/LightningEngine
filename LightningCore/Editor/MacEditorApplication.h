//
//  EditorApplication.hpp
//  LightningEditor
//
//  Created by Kian Marvi on 7/11/25.
//

#ifndef EditorApplication_hpp
#define EditorApplication_hpp

#include "MacEditorDelegate.h"

class MacEditorApplication
{
public:
    explicit MacEditorApplication(float p_Width = 1920.f, float p_Height = 1080.f, const char* p_Title = "Lightning Editor");
    ~MacEditorApplication();
    void Update();

private:
    NS::Application* m_SharedApplication = nullptr;
    MacEditorDelegate m_MacEditorDelagate;
};

#endif /* EditorApplication_hpp */
