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
    
    std::vector<const char*> CubeTextures =
    {
        "Assets/Textures/brickwall.jpg",
        nullptr,
        "Assets/Textures/brickwall_normal.jpg",
    };
    
    std::vector<const char*> PlaneTextures =
    {
        "Assets/Textures/brickwall.jpg",
        nullptr,
        "Assets/Textures/brickwall_normal.jpg",
    };
    
    std::vector<const char*> SphereTextures =
    {
        "Assets/Textures/brick-color.png",
        nullptr,
        "Assets/Textures/brick-normal.png",
    };
    
    std::array<const char*, 6> Skybox =
    {
        "Assets/Textures/skybox/px.png",
        "Assets/Textures/skybox/nx.png",
        "Assets/Textures/skybox/py.png",
        "Assets/Textures/skybox/ny.png",
        "Assets/Textures/skybox/pz.png",
        "Assets/Textures/skybox/nz.png"
    };
    
    Entity sphere = m_Scene.CreateEntity("Sphere");
    sphere.AddComponent<TransformComponent>(float3(-3.0f, 0.0f, 0.0f));
    sphere.AddComponent<TextureComponent>(SphereTextures, m_MacWindow.GetDevice());
    sphere.AddComponent<MeshComponent>(m_MetalRenderer.Create3DMesh(MeshBuilder::GenerateSphere(32, 32, float3(1.0f, 1.0f, 1.0f)), sphere.GetComponent<TextureComponent>().texture));
    
    
    
    Entity cube = m_Scene.CreateEntity("Cube");
    cube.AddComponent<TransformComponent>(float3(1.0f, 0.0f, 0.0f));
    cube.AddComponent<TextureComponent>(CubeTextures, m_MacWindow.GetDevice());
    cube.AddComponent<MeshComponent>(m_MetalRenderer.Create3DMesh(MeshBuilder::GenerateCube(), cube.GetComponent<TextureComponent>().texture));
    
    
    Entity plane = m_Scene.CreateEntity("Plane");
    plane.AddComponent<TransformComponent>(float3(-1.0f, -1.0f, -1.0f), -90.f, float3(1.0f, 0.0f, 0.0f));
    plane.AddComponent<TextureComponent>(PlaneTextures, m_MacWindow.GetDevice());
    plane.AddComponent<MeshComponent>(m_MetalRenderer.Create3DMesh(MeshBuilder::GeneratePlane(), plane.GetComponent<TextureComponent>().texture));
    
    
    
    Entity lightCube = m_Scene.CreateEntity("Light Cube");
    lightCube.AddComponent<TransformComponent>(float3(-1.0f, 1.0f, 2.0f), float3(0.2f,0.2f, 0.2f));
    lightCube.AddComponent<LightComponent>(float3(1.0f, 1.0f, 1.0f));
    lightCube.AddComponent<MeshComponent>(m_MetalRenderer.Create3DMesh(MeshBuilder::GenerateCube()));
    
    Entity skybox = m_Scene.CreateEntity("Skybox");
    skybox.AddComponent<TransformComponent>(float3(0.0f, 0.0f, 0.0f), float3(1000.0f, 1000.0f, 1000.0f));
    skybox.AddComponent<TextureComponent>(Skybox, m_MacWindow.GetDevice());
    skybox.AddComponent<MeshComponent>(m_MetalRenderer.Create3DMesh(MeshBuilder::GenerateCube()));
    skybox.AddComponent<SkyboxComponent>();
    
    m_CameraEntity = m_Scene.CreateEntity("Camera Entity");
    m_CameraEntity.AddComponent<TransformComponent>(float3(0.0f, 0.0f, 0.0f));
    m_CameraEntity.AddComponent<CameraComponent>();
    m_CameraEntity.GetComponent<CameraComponent>().b_Primary = true;
    
    // This needs to be removed. Application classes should never have to worry about managing GPU residency.
    m_MetalRenderer.AddToResidencySet(skybox.GetComponent<TextureComponent>().texture.GetCubeMap());
    m_MetalRenderer.AddToResidencySet(skybox.GetComponent<TextureComponent>().texture.GetArgumentBuffer());
    
    m_MetalRenderer.CommitResidencySet();
}


void MacApplication::Update(float p_DeltaTime)
{
    while (m_MacWindow.Update())
    {
        NS::AutoreleasePool* m_Pool = NS::AutoreleasePool::alloc()->init();
        m_CurrentFrame = (float)glfwGetTime();
        m_DeltaTime = m_CurrentFrame - m_LastFrame;
        m_LastFrame = m_CurrentFrame;
        
        if (m_CameraEntity.GetComponent<CameraComponent>().b_Primary)
        {
            m_CameraController.Update(m_CameraEntity.GetComponent<CameraComponent>().m_Camera, m_DeltaTime);
        }
        
        m_Camera.ProcessControllerLeftThumbstickInput(m_DeltaTime, m_Controller.LeftThumbstickX(), m_Controller.LeftThumbstickY());
        
        m_Camera.ProcessControllerRightThumbstickInput(m_Controller.RightThumbstickX(), m_Controller.RightThumbstickY());
        
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
        
        m_Scene.RenderScene(m_MetalRenderer, m_MetalFrameBuffer.GetWidth() / m_MetalFrameBuffer.GetHeight());
        
        m_WindowDrawable->present();
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
