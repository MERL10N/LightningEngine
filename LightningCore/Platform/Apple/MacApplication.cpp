//
//  MacApplication.cpp
//  LightningCore
//  Created by Kian Marvi on 7/3/25.
//

#include "MacApplication.h"
#include "QuartzCore/QuartzCore.hpp"
#include "Metal/Metal.hpp"


MacApplication::MacApplication(unsigned int p_Width, unsigned int p_Height, const char* p_Title)
: m_MacWindow(p_Width, p_Height, p_Title),
  m_MetalRenderer(m_MacWindow.GetDevice(), m_MacWindow.GetMetalLayer()),
  m_WindowPassDescriptor(MTL::RenderPassDescriptor::alloc()->init()),
  m_WindowDrawable(m_MacWindow.GetMetalLayer()->nextDrawable())
{
    m_MetalRenderer.CreateQuad("../../../Assets/megaman.png");
}


void MacApplication::Update(float p_DeltaTime)
{
    while (m_MacWindow.Update())
    {
        NS::AutoreleasePool* m_Pool = NS::AutoreleasePool::alloc()->init();
        {
            m_WindowPassDescriptor->colorAttachments()->object(0)->setTexture(m_WindowDrawable->texture());
            m_WindowPassDescriptor->colorAttachments()->object(0)->setLoadAction(MTL::LoadActionClear);
            m_WindowPassDescriptor->colorAttachments()->object(0)->setClearColor(MTL::ClearColor::Make(0.15, 0.15, 0.15, 1));
            m_WindowPassDescriptor->colorAttachments()->object(0)->setStoreAction(MTL::StoreActionStore);
            
            m_MetalRenderer.BeginFrame();
            m_MetalRenderer.SetRenderPassDescriptor(m_WindowPassDescriptor);
            m_MetalRenderer.Render();
            m_MetalRenderer.GetMetalCommandBuffer()->presentDrawable(m_WindowDrawable);
            m_MetalRenderer.Commit();
        }
        m_Pool->release();
    }
}


MacApplication::~MacApplication()
{
    if (m_WindowPassDescriptor)
    {
        m_WindowPassDescriptor->release();
        m_WindowPassDescriptor = nullptr;
    }
}


