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
#include <simd/simd.h>

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
    
    MeshBuilder m_MeshBuilder;
    
    m_MetalRenderer.AddToResidencySet(m_MetalFrameBuffer.GetAttachmentTexture());
    
    Entity cube = m_Scene.CreateEntity("Cube");
    cube.AddComponent<TransformComponent>(simd::make_float3(0.0f, 0.0f, 0.0f));
    cube.AddComponent<MeshComponent>(m_MeshBuilder.GenerateCube(m_MetalDevice));
    cube.AddComponent<TextureComponent>("Assets/Textures/background.png", m_MetalDevice);
    m_MetalRenderer.RegisterMesh(cube.GetComponent<MeshComponent>().m_Mesh);
    m_MetalRenderer.RegisterTexture(cube.GetComponent<TextureComponent>().m_Texture);
    
    
    Entity plane = m_Scene.CreateEntity("Plane");
    plane.AddComponent<TransformComponent>(simd::make_float3(0.0f, -2.0f, 0.0f), simd::make_float3(10.0f, 0.1f, 10.0f));
    plane.AddComponent<MeshComponent>(m_MeshBuilder.GenerateCube(m_MetalDevice));
    m_MetalRenderer.RegisterMesh(plane.GetComponent<MeshComponent>().m_Mesh);
     
    Entity sphere = m_Scene.CreateEntity("Sphere");
    sphere.AddComponent<TransformComponent>(simd::make_float3(-5.0f, 0.0f, 0.0f));
    sphere.AddComponent<LightComponent>(simd::make_float3(1.0f, 1.0f, 1.0f));
    sphere.AddComponent<MeshComponent>(m_MeshBuilder.GenerateSphere(m_MetalDevice, 32, 32, simd::make_float3(1.0f, 1.0f, 1.0f)));
    m_MetalRenderer.RegisterMesh(sphere.GetComponent<MeshComponent>().m_Mesh);
    m_MetalRenderer.CommitResidencySet();
    
    m_CameraEntity = m_Scene.CreateEntity("Camera Entity");
    m_CameraEntity.AddComponent<CameraComponent>();
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
            m_Scene.RenderScene(m_MetalRenderer, m_Camera, m_MetalFrameBuffer.GetWidth() / m_MetalFrameBuffer.GetHeight());

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


