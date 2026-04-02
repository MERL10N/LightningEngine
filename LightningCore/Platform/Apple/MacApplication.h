//
//  MacApplication.hpp
//  LightningCore
//
//  Created by Kian Marvi on 7/3/25.
//

#ifndef MacApplication_hpp
#define MacApplication_hpp

#include "MacWindow.h"
#include "Camera/Camera.h"
#include "Input/AppleController.h"

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
    MetalRenderer* m_MetalRenderer = nullptr;
    MTL::RenderPassDescriptor* m_WindowPassDescriptor = nullptr;
    CA::MetalDrawable* m_WindowDrawable = nullptr;
    Camera m_Camera;
    AppleController m_Controller;
    float m_CurrentFrame = 0.0f;
    float m_DeltaTime = 0.0f;
    float m_LastFrame = 0.0f;
};

#endif /* MacApplication_hpp */
