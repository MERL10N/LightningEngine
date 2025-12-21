//
//  MacApplication.hpp
//  LightningCore
//
//  Created by Kian Marvi on 7/3/25.
//

#ifndef MacApplication_hpp
#define MacApplication_hpp

#include "MacWindow.h"
#include "Renderer/Metal/MetalRenderer.h"
class MetalRenderer;

namespace MTL
{
    class RenderPassDescriptor;
}

namespace CA
{
  class MetalDrawable;
}


class MacApplication
{
public:
    MacApplication(unsigned int p_Width = 1920, unsigned int p_Height = 1080, const char* p_Title = "Lightning Game");
    ~MacApplication();
    void Update(float p_DeltaTime);

private:
    MacWindow m_MacWindow;
    MetalRenderer m_MetalRenderer;
    MTL::RenderPassDescriptor* m_WindowPassDescriptor = nullptr;
    CA::MetalDrawable* m_WindowDrawable = nullptr;
    float m_AspectRatio;
};

#endif /* MacApplication_hpp */
