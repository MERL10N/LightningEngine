//
//  EditorApplication.hpp
//  LightningEditor
//
//  Created by Kian Marvi on 7/11/25.
//

#ifndef EditorApplication_hpp
#define EditorApplication_hpp

#include "Editor/MacEditorDelegate.hpp"

class MacEditorApplication
{
public:
    MacEditorApplication(float p_Width = 1920.f, float p_Height = 1080.f, const char* p_Title = "");
    ~MacEditorApplication();
    void Update();

private:
    NS::Application* m_SharedApplication;
    MacEditorDelegate m_MacEditorApp;
};

#endif /* EditorApplication_hpp */
