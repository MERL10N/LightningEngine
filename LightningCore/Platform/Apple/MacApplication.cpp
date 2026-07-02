//
//  MacApplication.cpp
//  LightningCore
//  Created by Kian Marvi on 7/3/25.
//

#include "MacApplication.h"
#include "QuartzCore/QuartzCore.hpp"
#include "Metal/Metal.hpp"
#include "GLFW/glfw3.h"
#include "Scene/Component.h"
#include "Primitives/MeshBuilder.h"
#include "Entity/Entity.h"

MacApplication::MacApplication(unsigned int p_Width, unsigned int p_Height, const char* p_Title)
: m_MacWindow(p_Width, p_Height, p_Title),
  m_MetalRenderer(m_MacWindow.GetDevice(), m_MacWindow.GetMetalLayer()),
  m_WindowPassDescriptor(MTL4::RenderPassDescriptor::alloc()->init()),
  m_Camera(Camera()),
  m_Scene()
{
    MeshBuilder m_MeshBuilder;
    
    Entity cube = m_Scene.CreateEntity("Cube");
    cube.AddComponent<TransformComponent>(simd::make_float3(0.0f, 0.0f, 0.0f));
    cube.AddComponent<MeshComponent>(m_MeshBuilder.GenerateCube(m_MacWindow.GetDevice()));
    cube.AddComponent<TextureComponent>("Assets/Textures/background.png", m_MacWindow.GetDevice());
    
    Entity sphere = m_Scene.CreateEntity("Sphere");
    sphere.AddComponent<TransformComponent>(simd::make_float3(-5.0f, 0.0f, 0.0f));
    sphere.AddComponent<LightComponent>(simd::make_float3(1.0f, 1.0f, 1.0f));
    sphere.AddComponent<MeshComponent>(m_MeshBuilder.GenerateSphere(m_MacWindow.GetDevice(), 32, 32, simd::make_float3(1.0f, 1.0f, 1.0f)));
}


void MacApplication::Update(float p_DeltaTime)
{
    while (m_MacWindow.Update())
    {
        m_CurrentFrame = (float)glfwGetTime();
        m_DeltaTime = m_CurrentFrame - m_LastFrame;
        m_LastFrame = m_CurrentFrame;
        
        if (m_Controller.IsWKeyDown())
            m_Camera.ProcessKeyboardInput(CAMERA_MOVEMENT::FORWARD, m_DeltaTime);
        if (m_Controller.IsSKeyDown())
            m_Camera.ProcessKeyboardInput(CAMERA_MOVEMENT::BACKWARD, m_DeltaTime);
        if (m_Controller.IsAKeyDown())
            m_Camera.ProcessKeyboardInput(CAMERA_MOVEMENT::LEFT, m_DeltaTime);
        if (m_Controller.IsDKeyDown())
            m_Camera.ProcessKeyboardInput(CAMERA_MOVEMENT::RIGHT, m_DeltaTime);
     
        m_Camera.ProcessControllerLeftThumbstickInput(m_DeltaTime, m_Controller.LeftThumbstickX(), m_Controller.LeftThumbstickY());
        
        m_Camera.ProcessControllerRightThumbstickInput(m_Controller.RightThumbstickX(), m_Controller.RightThumbstickY());
        NS::AutoreleasePool* m_Pool = NS::AutoreleasePool::alloc()->init();
        {
            m_WindowDrawable = m_MacWindow.GetMetalLayer()->nextDrawable();
            m_WindowPassDescriptor->colorAttachments()->object(0)->setTexture(m_WindowDrawable->texture());
            m_WindowPassDescriptor->colorAttachments()->object(0)->setLoadAction(MTL::LoadActionClear);
            m_WindowPassDescriptor->colorAttachments()->object(0)->setClearColor(MTL::ClearColor::Make(0.15, 0.15, 0.15, 1));
            m_WindowPassDescriptor->colorAttachments()->object(0)->setStoreAction(MTL::StoreActionStore);
            
            m_MetalRenderer.SetMetalDrawable(m_WindowDrawable);
            m_MetalRenderer.SetRenderPassDescriptor(m_WindowPassDescriptor);
            m_Scene.RenderScene(m_MetalRenderer, m_Camera,  m_MacWindow.GetWidth() / m_MacWindow.GetHeight());
            m_WindowDrawable->present();
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


