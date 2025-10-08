//
//  EditorApplication.hpp
//  LightningEditor
//
//  Created by Kian Marvi on 7/11/25.
//

#ifndef EditorApplication_hpp
#define EditorApplication_hpp

#include "EditorLayer.h"
#include "Platform/Apple/MacWindow.h"

class MetalRenderer;

class MacEditorApplication
{
public:
    explicit MacEditorApplication(float p_Width = 1920.f, float p_Height = 1080.f, const char* p_Title = "Lightning Editor");
    ~MacEditorApplication();
    void Update();

private:

   // EditorLayer m_EditorLayer;
    MacWindow m_MacWindow;
    MetalRenderer* m_MetalRenderer;
};

#endif /* EditorApplication_hpp */
