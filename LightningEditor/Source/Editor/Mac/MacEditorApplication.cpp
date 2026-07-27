//
//  EditorApplication.cpp
//  LightningEditor
//
//  Created by Kian Marvi on 7/11/25.
//

#include "MacEditorApplication.h"
#include "Scene/Component.h"
#include "Primitives/MeshBuilder.h"

#include <imgui/imgui.h>
#include <imgui/backends/imgui_impl_glfw.h>
#include <imgui/backends/imgui_impl_metal4.h>

#include <GLFW/glfw3.h>

#include <QuartzCore/QuartzCore.hpp>
#include <Metal/Metal.hpp>
#include <hlsl++.h>

MacEditorApplication::MacEditorApplication(const float p_Width, const float p_Height, const char* p_Title)
: m_MacWindow(p_Width, p_Height, p_Title),
  m_MetalDevice(m_MacWindow.GetDevice()),
  m_MacEditorLayer(m_MetalDevice),
  m_MetalRenderer(m_MetalDevice, m_MacWindow.GetMetalLayer()),
  m_MetalFrameBuffer(m_MetalDevice),
  m_WindowPassDescriptor(MTL4::RenderPassDescriptor::alloc()->init()),
  m_ImGuiCommandAllocator(m_MetalDevice->newCommandAllocator()),
  m_Camera(),
  m_Scene(),
  m_Width(p_Width),
  m_Height(p_Height)
{
    
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO();
    (void)io;
    
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
    io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;         // Enable Docking
    io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;       // Enable viewports
    io.ConfigFlags |= ImGuiWindowFlags_NoResize;
    
    // Setup style
    ImGui::StyleColorsDark();
    ImGui_ImplGlfw_InitForOther(m_MacWindow.GetWindow(), true);
    ImGui_ImplMetal4_Init(m_MacWindow.GetDevice(), m_MetalRenderer.GetMetalCommandQueue(), 3);
    io.Fonts->AddFontFromFileTTF("Assets/Fonts/JetBrainsMono-Light.ttf");
    
    m_MetalFrameBuffer.Create(m_Width, m_Height);
    m_MetalRenderer.AddToResidencySet(m_MetalFrameBuffer.GetAttachmentTexture());
    
    
    std::vector<const char*> CubeTextures =
    {
        "Assets/Textures/container2.png",
        "Assets/Textures/container2_specular.png"
    };
    
    std::vector<const char*> PlaneTextures =
    {
        "Assets/Textures/brickwall.jpg",
        nullptr,
        "Assets/Textures/brickwall_normal.jpg",
    };
    
    
    
    Entity cube = m_Scene.CreateEntity("Cube");
    cube.AddComponent<TransformComponent>(float3(0.0f, 0.0f, 0.0f));
    cube.AddComponent<TextureComponent>(CubeTextures, m_MacWindow.GetDevice());
    cube.AddComponent<MeshComponent>(m_MetalRenderer.Create3DMesh(MeshBuilder::GenerateCube(), cube.GetComponent<TextureComponent>().texture));
    
    
    /*
    Entity lightSphere = m_Scene.CreateEntity("Light Sphere");
    lightSphere.AddComponent<TransformComponent>(float3(-1.0f, 1.0f, 2.0f), float3(0.2f,0.2f, 0.2f));
    lightSphere.AddComponent<LightComponent>(float3(1.0f, 1.0f, 1.0f));
    lightSphere.AddComponent<MeshComponent>(m_MetalRenderer.Create3DMesh(MeshBuilder::GenerateSphere(32, 32, float3(1.0f, 1.0f, 1.0f)), nullptr));
     */
    
    
    Entity plane = m_Scene.CreateEntity("Plane");
    plane.AddComponent<TransformComponent>(float3(-1.0f, -1.0f, -1.0f), -90.f, float3(1.0f, 0.0f, 0.0f));
    plane.AddComponent<TextureComponent>(PlaneTextures, m_MacWindow.GetDevice());
    plane.AddComponent<MeshComponent>(m_MetalRenderer.Create3DMesh(MeshBuilder::GeneratePlane(), plane.GetComponent<TextureComponent>().texture));
    
    Entity lightCube = m_Scene.CreateEntity("Light Cube");
    lightCube.AddComponent<TransformComponent>(float3(-1.0f, 1.0f, 2.0f), float3(0.2f,0.2f, 0.2f));
    lightCube.AddComponent<LightComponent>(float3(1.0f, 1.0f, 1.0f));
    lightCube.AddComponent<MeshComponent>(m_MetalRenderer.Create3DMesh(MeshBuilder::GenerateCube(), nullptr));
    m_MetalRenderer.CommitResidencySet();
}


MacEditorApplication::~MacEditorApplication()
{
    // Cleanup
    ImGui_ImplMetal4_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
    
    if (m_WindowPassDescriptor)
    {
        m_WindowPassDescriptor->release();
        m_WindowPassDescriptor = nullptr;
    }
    
    if (m_ImGuiCommandAllocator)
    {
        m_ImGuiCommandAllocator->release();
        m_ImGuiCommandAllocator = nullptr;
    }
}


void MacEditorApplication::DrawGameViewport()
{
    // Prevent crashes when compiling for the first time
    if (m_ViewportSize.x == 0.0f || m_ViewportSize.y == 0.0f)
        ImGui::SetNextWindowSize(ImVec2(m_Width, m_Height), ImGuiCond_FirstUseEver);
        
    ImGui::Begin("Game Scene");
    {
        ImVec2 viewportPanelSize = ImGui::GetContentRegionAvail();
      
        if (m_ViewportSize.x != viewportPanelSize.x || m_ViewportSize.y != viewportPanelSize.y)
        {
            m_ViewportSize.x = viewportPanelSize.x;
            m_ViewportSize.y = viewportPanelSize.y;
            m_MetalFrameBuffer.Resize(m_ViewportSize.x, m_ViewportSize.y);
        }
        
        ImVec2 pos = ImGui::GetCursorScreenPos();
        
        ImGui::GetWindowDrawList()->AddImage(m_MetalFrameBuffer.GetAttachmentTexture(),
                                             ImVec2(pos.x, pos.y),
                                             ImVec2(pos.x + m_ViewportSize.x, pos.y + m_ViewportSize.y),
                                             ImVec2(0, 0),ImVec2(1, 1));
    }
    ImGui::End();
}

void MacEditorApplication::Update()
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

            ImGuiIO& io = ImGui::GetIO();

            m_WindowDrawable = m_MacWindow.GetMetalLayer()->nextDrawable();

            m_WindowPassDescriptor->colorAttachments()->object(0)->setTexture(m_WindowDrawable->texture());
            m_WindowPassDescriptor->colorAttachments()->object(0)->setLoadAction(MTL::LoadActionClear);
            m_WindowPassDescriptor->colorAttachments()->object(0)->setClearColor(MTL::ClearColor::Make(0.15, 0.15, 0.15, 1));
            m_WindowPassDescriptor->colorAttachments()->object(0)->setStoreAction(MTL::StoreActionStore);

            m_MetalRenderer.SetMetalDrawable(m_WindowDrawable);

            ImGui_ImplMetal4_NewFrame(m_WindowPassDescriptor, 0);
            ImGui_ImplGlfw_NewFrame();
            ImGui::NewFrame();

            ImGui::DockSpaceOverViewport();

            DrawGameViewport();

            m_MacEditorLayer.DrawContentBrowser();
            m_MacEditorLayer.DrawMenuBar();
            m_MacEditorLayer.DrawStatsBar();

            b_EnableWireframe = m_MacEditorLayer.IsWireFrameEnabled();

            m_MetalRenderer.SetWireframeMode(b_EnableWireframe);
             
            // Submit FrameBuffer To Renderer
            m_MetalRenderer.SetRenderPassDescriptor(m_MetalFrameBuffer.GetRenderPassDescriptor());
            
            float aspectRatio = m_MetalFrameBuffer.GetWidth() / m_MetalFrameBuffer.GetHeight();
            m_Scene.RenderScene(m_MetalRenderer, m_Camera, aspectRatio);

            // Rendering
            ImGui::Render();


            // Render ImGui UI and Viewport
            m_ImGuiCommandBuffer = m_MetalRenderer.GetMetalCommandBuffer();
            m_ImGuiCommandAllocator->reset();
            m_ImGuiCommandBuffer->beginCommandBuffer(m_ImGuiCommandAllocator);
            m_ImGuiCommandEncoder = m_ImGuiCommandBuffer->renderCommandEncoder(m_WindowPassDescriptor);
            m_MetalFrameBuffer.UpdateViewport(m_ImGuiCommandEncoder);
            ImGui_ImplMetal4_RenderDrawData(ImGui::GetDrawData(), m_ImGuiCommandBuffer, m_ImGuiCommandEncoder);
            
            if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
            {
                ImGui::UpdatePlatformWindows();
                ImGui::RenderPlatformWindowsDefault();
            }
            m_ImGuiCommandEncoder->endEncoding();
            m_ImGuiCommandBuffer->endCommandBuffer();
            m_MetalRenderer.GetMetalCommandQueue()->commit(&m_ImGuiCommandBuffer, 1);
            m_ImGuiCommandAllocator->reset();
            m_WindowDrawable->present();
         
        }
                 
        m_Pool->release();
    }
}


