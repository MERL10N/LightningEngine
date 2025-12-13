//
//  EditorApplication.cpp
//  LightningEditor
//
//  Created by Kian Marvi on 7/11/25.
//

#include "MacEditorApplication.h"
#include "Renderer/Metal/MetalRenderer.h"

MacEditorApplication::MacEditorApplication(float p_Width, float p_Height, const char* p_Title)
: m_MacWindow(p_Width, p_Height, p_Title),
  m_MetalRenderer(new MetalRenderer(m_MacWindow.GetDevice(), m_MacWindow.GetMetalLayer()))
{
}


void MacEditorApplication::Update()
{
        while (m_MacWindow.Update())
        {
            NS::AutoreleasePool* m_Pool = NS::AutoreleasePool::alloc()->init();
            {
                m_MetalRenderer->BeginFrame();
                m_MetalRenderer->Render();
                m_MetalRenderer->Commit();
            }
            m_Pool->release();
        }
}


MacEditorApplication::~MacEditorApplication()
{
    if (m_MetalRenderer)
    {
        delete m_MetalRenderer;
        m_MetalRenderer = nullptr;
    }
}
