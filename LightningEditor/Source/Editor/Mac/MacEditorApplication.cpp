//
//  EditorApplication.cpp
//  LightningEditor
//
//  Created by Kian Marvi on 7/11/25.
//

#include "MacEditorApplication.h"
#include "MacEditorLayer.h"
#include "Renderer/Metal/MetalRenderer.h"
#include "Renderer/Metal/MetalFrameBuffer.h"
#include "imgui/imgui.h"
#include "imgui/backends/imgui_impl_glfw.h"
#include "imgui/backends/imgui_impl_metal.h"
#include "QuartzCore/QuartzCore.hpp"
#include "Scene/Scene.h"
#include "Scene/Component.h"
#include "Entity/Entity.h"
#include "GLFW/glfw3.h"
#include <simd/simd.h>

MacEditorApplication::MacEditorApplication(float p_Width, float p_Height, const char* p_Title)
: m_MacWindow(p_Width, p_Height, p_Title),
  m_MetalDevice(m_MacWindow.GetDevice()),
  m_MacEditorLayer(new MacEditorLayer(m_MetalDevice)),
  m_MetalRenderer(new MetalRenderer(m_MetalDevice, m_MacWindow.GetMetalLayer())),
  m_MetalFrameBuffer(m_MetalDevice),
  m_WindowPassDescriptor(MTL::RenderPassDescriptor::alloc()->init()),
  m_Camera(Camera()),
  m_Scene(new Scene())
{
    /*
    m_MetalRenderer->CreateQuad("Assets/Textures/city/1.png", simd::make_float3(50.f, 50.f, 1.f), simd::make_float3(0.0f,5.f, -4.f));
    m_MetalRenderer->CreateQuad("Assets/Textures/city/2.png", simd::make_float3(10.f, 10.f, 1.f), simd::make_float3(0.0f,6.f, -3.9f));
    m_MetalRenderer->CreateQuad("Assets/Textures/city/3.png", simd::make_float3(20.f, 20.f, 1.f), simd::make_float3(0.0f,5.5f, -3.8f));
    m_MetalRenderer->CreateQuad("Assets/Textures/city/4.png", simd::make_float3(20.f, 20.f, 1.f), simd::make_float3(0.0f,0.f, -3.7f));
    m_MetalRenderer->CreateQuad("Assets/Textures/city/5.png", simd::make_float3(20.f, 20.f, 1.f), simd::make_float3(0.0f,0.f, -3.6f));
    m_MetalRenderer->CreateQuad("Assets/Textures/city/8.png", simd::make_float3(20.f, 20.f, 1.f), simd::make_float3(0.0f,0.f, -3.f));
    m_MetalRenderer->CreateQuad("Assets/Textures/megaman.png", simd::make_float3(1.f, 1.f, 1.f), simd::make_float3(0.0f,-5.0f, -1.0f));

    //auto square = m_Scene.CreateEntity();
    //m_Scene.Reg().emplace<TransformComponent>(square);
    //m_Scene.Reg().emplace<SpriteRendererComponent>(square, simd::make_float4(0.0f, 1.0f, 0.0f, 1.0f));
     */
    
    auto cube = m_Scene->CreateEntity("Cube");
    cube.GetComponent<TransformComponent>().m_Transform = matrix4x4_translation(simd::make_float3(0.0f, 0.0f, 0.0f));
    cube.AddComponent<MeshComponent>(m_MeshBuilder.GenerateCube(m_MetalDevice, "Assets/Textures/background.png"));
    
    auto sphere = m_Scene->CreateEntity("Sphere");
    sphere.GetComponent<TransformComponent>().m_Transform = matrix4x4_translation(simd::make_float3(-5.0f, 0.0f, 0.0f));
    sphere.AddComponent<MeshComponent>(m_MeshBuilder.GenerateSphere(m_MetalDevice, 64, 64));;
    
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO();
    (void)io;
    
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
    io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;         // Enable Docking
    io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;       // Enable viewports
    
    // Setup style
    ImGui::StyleColorsDark();
    ImGui_ImplGlfw_InitForOther(m_MacWindow.GetWindow(), true);
    ImGui_ImplMetal_Init(m_MacWindow.GetDevice());
    io.Fonts->AddFontFromFileTTF("Assets/Fonts/JetBrainsMono-Light.ttf");
    
    m_MetalFrameBuffer.Create(p_Width, p_Height);
}


MacEditorApplication::~MacEditorApplication()
{
    // Cleanup
    ImGui_ImplMetal_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();

    if (m_MetalRenderer)
    {
        delete m_MetalRenderer;
        m_MetalRenderer = nullptr;
    }
    
    if (m_WindowPassDescriptor)
    {
        m_WindowPassDescriptor->release();
        m_WindowPassDescriptor = nullptr;
    }
    
    if (m_MacEditorLayer)
    {
        delete m_MacEditorLayer;
        m_MacEditorLayer = nullptr;
    }
    
    if (m_Scene)
    {
        delete m_Scene;
        m_Scene = nullptr;
    }
}


void MacEditorApplication::DrawGameViewport()
{
    // Prevent crashes when compiling for the first time
    if (m_ViewportSize.x == 0.0f || m_ViewportSize.y == 0.0f)
        ImGui::SetNextWindowSize(ImVec2(2560, 1600), ImGuiCond_FirstUseEver);
        
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

            m_Scene->Update(m_DeltaTime);
            
            NS::AutoreleasePool* m_Pool = NS::AutoreleasePool::alloc()->init();
            {
                
                ImGuiIO& io = ImGui::GetIO();
                
                m_WindowDrawable = m_MacWindow.GetMetalLayer()->nextDrawable();
                
                m_WindowPassDescriptor->colorAttachments()->object(0)->setTexture(m_WindowDrawable->texture());
                m_WindowPassDescriptor->colorAttachments()->object(0)->setLoadAction(MTL::LoadActionClear);
                m_WindowPassDescriptor->colorAttachments()->object(0)->setClearColor(MTL::ClearColor::Make(0, 0, 0, 1));
                m_WindowPassDescriptor->colorAttachments()->object(0)->setStoreAction(MTL::StoreActionStore);
                
                ImGui_ImplMetal_NewFrame(m_WindowPassDescriptor);
                ImGui_ImplGlfw_NewFrame();
                ImGui::NewFrame();
                
                ImGui::DockSpaceOverViewport();
                
                DrawGameViewport();
                
                m_MacEditorLayer->DrawContentBrowser();
                m_MacEditorLayer->DrawMenuBar();
                m_MacEditorLayer->DrawStatsBar();
                
                // Submit FrameBuffer To Renderer
                m_MetalRenderer->SetCamera(m_Camera);
                m_MetalRenderer->BeginFrame();
                m_MetalRenderer->SetRenderPassDescriptor(m_MetalFrameBuffer.GetRenderPassDescriptor());
                m_MetalRenderer->Render(m_Scene);
                m_MetalRenderer->Commit();
                
                // Rendering
                ImGui::Render();

               // Render ImGui UI and Viewport
               m_MetalRenderer->BeginFrame();
               m_ImGuiCommandBuffer = m_MetalRenderer->GetMetalCommandBuffer();
               m_ImGuiCommandEncoder = m_ImGuiCommandBuffer->renderCommandEncoder(m_WindowPassDescriptor);
               m_MetalFrameBuffer.UpdateViewport(m_ImGuiCommandEncoder);
               ImGui_ImplMetal_RenderDrawData(ImGui::GetDrawData(), m_ImGuiCommandBuffer, m_ImGuiCommandEncoder);
               if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
               {
                   ImGui::UpdatePlatformWindows();
                   ImGui::RenderPlatformWindowsDefault();
               }
                m_ImGuiCommandEncoder->endEncoding();
                m_ImGuiCommandBuffer->presentDrawable(m_WindowDrawable);
                m_ImGuiCommandBuffer->commit();
            }
            m_Pool->release();
        }
}


