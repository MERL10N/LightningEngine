//
//  EditorApplication.cpp
//  LightningEditor
//
//  Created by Kian Marvi on 7/11/25.
//

#include "MacEditorApplication.h"

MacEditorApplication::MacEditorApplication(float p_Width, float p_Height, const char* p_Title)
: m_MacEditorApp(p_Width, p_Height, p_Title)
{
    m_SharedApplication = NS::Application::sharedApplication();
    m_SharedApplication->setDelegate(&m_MacEditorApp);
}


void MacEditorApplication::Update()
{
    m_SharedApplication->run();
}


MacEditorApplication::~MacEditorApplication()
{
    m_SharedApplication->release();
}
