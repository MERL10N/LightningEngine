//
//  MacApplication.cpp
//  LightningCore
//  Created by Kian Marvi on 7/3/25.
//

#include "MacApplication.h"
#include "Renderer/Metal/MetalRenderer.h"
#include "GLFW/glfw3.h"


MacApplication::MacApplication(unsigned int p_Width, unsigned int p_Height, const char* p_Title)
: m_MacWindow(p_Width, p_Height, p_Title),
  m_MetalRenderer(new MetalRenderer(m_MacWindow.GetDevice(), m_MacWindow.GetMetalLayer()))
{
}


void MacApplication::Update(float p_DeltaTime)
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


MacApplication::~MacApplication()
{
    if (m_MetalRenderer)
    {
        delete m_MetalRenderer;
        m_MetalRenderer = nullptr;
    }
}


