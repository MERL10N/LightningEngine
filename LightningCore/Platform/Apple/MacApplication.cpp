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
  m_MetalFrameBuffer(m_MacWindow.GetDevice()),
  m_Camera(),
  m_Scene()
{
    MeshBuilder m_MeshBuilder;
    
    m_MetalFrameBuffer.Create(p_Width, p_Height);
    m_MetalRenderer.AddToResidencySet(m_MetalFrameBuffer.GetAttachmentTexture());
    
    Entity cube = m_Scene.CreateEntity("Cube");
    cube.AddComponent<TransformComponent>(float3(0.0f, 0.0f, 0.0f));
    cube.AddComponent<TextureComponent>("Assets/Textures/background.png", m_MacWindow.GetDevice());
    cube.AddComponent<MeshComponent>(m_MetalRenderer.Create3DMesh(MeshBuilder::GenerateCube(), cube.GetComponent<TextureComponent>().texture));
    
    
    Entity plane = m_Scene.CreateEntity("Plane");
    plane.AddComponent<TransformComponent>(float3(-2.0f, -2.0f, 0.0f), float3(10.0f, 0.1f, 10.0f));
    plane.AddComponent<MeshComponent>(m_MetalRenderer.Create3DMesh(MeshBuilder::GenerateCube(), nullptr));
     
    Entity sphere = m_Scene.CreateEntity("Sphere");
    sphere.AddComponent<TransformComponent>(float3(-5.0f, 0.0f, 0.0f));
    sphere.AddComponent<LightComponent>(float3(1.0f, 1.0f, 1.0f));
    sphere.AddComponent<MeshComponent>(m_MetalRenderer.Create3DMesh(MeshBuilder::GenerateSphere(32, 32, float3(1.0f, 1.0f, 1.0f)), nullptr));
    m_MetalRenderer.CommitResidencySet();
    
    Entity sphere2 = m_Scene.CreateEntity("Sphere 2");
    sphere2.AddComponent<TransformComponent>(float3(2.5f, 0.0f, 0.0f));
    sphere2.AddComponent<LightComponent>(float3(0.0f, 1.0f, 1.0f));
    sphere2.AddComponent<MeshComponent>(m_MetalRenderer.Create3DMesh(MeshBuilder::GenerateSphere(32, 32, float3(1.0f, 1.0f, 1.0f)), nullptr));
    m_MetalRenderer.CommitResidencySet();
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
            
            float drawableWidth = m_WindowDrawable->texture()->width();
            float drawableHeight = m_WindowDrawable->texture()->height();
            
            if (m_MetalFrameBuffer.GetWidth() != drawableWidth || m_MetalFrameBuffer.GetHeight() != drawableHeight)
            {
                m_MetalFrameBuffer.Resize(drawableWidth, drawableHeight);
            }
            
            m_ColorAttachmentDescriptor = m_MetalFrameBuffer.GetRenderPassDescriptor()->colorAttachments()->object(0);
            m_ColorAttachmentDescriptor->setResolveTexture(m_WindowDrawable->texture());
            
            m_MetalRenderer.SetMetalDrawable(m_WindowDrawable);
            
            m_MetalRenderer.SetRenderPassDescriptor(m_MetalFrameBuffer.GetRenderPassDescriptor());
            
            m_Scene.RenderScene(m_MetalRenderer, m_Camera, m_MetalFrameBuffer.GetWidth() / m_MetalFrameBuffer.GetHeight());
            
            m_WindowDrawable->present();
        }
        m_Pool->release();
    }
}


MacApplication::~MacApplication()
{
    if (m_ColorAttachmentDescriptor)
    {
        m_ColorAttachmentDescriptor->release();
        m_ColorAttachmentDescriptor = nullptr;
    }
}
