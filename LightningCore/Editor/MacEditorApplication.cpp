//
//  EditorApplication.cpp
//  LightningEditor
//
//  Created by Kian Marvi on 7/11/25.
//

#include "MacEditorApplication.h"

MacEditorApplication::MacEditorApplication(float p_Width, float p_Height, const char* p_Title)
: m_MacEditorDelagate(p_Width, p_Height, p_Title),
  m_SharedApplication(NS::Application::sharedApplication())
{
    assert(m_SharedApplication != nullptr);
    m_SharedApplication->setDelegate(&m_MacEditorDelagate);
}


void MacEditorApplication::Update()
{
    m_SharedApplication->run();
}


MacEditorApplication::~MacEditorApplication()
{
    if (m_SharedApplication)
    {
        m_SharedApplication->release();
        m_SharedApplication = nullptr;
    }
}
