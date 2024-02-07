//  MetalApplication.cpp
//  Lightning Engine

#include "MetalApplication.h"
#include <RenderControl/ShaderManager.h>
#include "Source/Editor/Editor.h"

#include "VertexData.h"
#include "System/ImageLoader.h"
#include "Math/AAPLMathUtilities.h"
#include "GLFWBridge.h"

#include <Metal/Metal.hpp>
#include <QuartzCore/CAMetalLayer.hpp>
#include <QuartzCore/QuartzCore.hpp>

#include <iostream>

#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>


bool CMetalApplication::Init()
{
    // Detect if metal is supported on the target device
    metalDevice = MTL::CreateSystemDefaultDevice();
    
    // Initialise GLFW
    glfwInit();
    glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
    glfwWindow = glfwCreateWindow(1280, 720, "Lightning Engine", nullptr, nullptr);
    
    CEditor::GetInstance()->SetClearColor(0.15f, 0);
    CEditor::GetInstance()->SetClearColor(0.15f, 1);
    CEditor::GetInstance()->SetClearColor(0.15f, 2);
    CEditor::GetInstance()->SetClearColor(1.f, 3);
    
#ifdef DEBUG
    CEditor::GetInstance()->Init(metalDevice, glfwWindow);
#endif
    
    if (!glfwWindow)
    {
        glfwTerminate();
        exit(EXIT_FAILURE);
    }
    
    glfwSetWindowUserPointer(glfwWindow, this);
    glfwSetFramebufferSizeCallback(glfwWindow, frameBufferSizeCallback);
    int width, height;
    glfwGetFramebufferSize(glfwWindow, &width, &height);
    
    layer = CA::MetalLayer::layer();
    layer->setDevice(metalDevice);
    layer->setPixelFormat(MTL::PixelFormatBGRA8Unorm);
    layer->setDrawableSize(CGSizeMake(width, height));
    GLFWBridge::AddLayerToWindow(glfwWindow, layer);

    // Initialise the shader
    CShaderManager::GetInstance()->Add("Square",
                                       "Shaders/square.metal",
                                       "vertexShader",
                                       "fragmentShader",
                                        metalDevice);

    // Render the square
    createSquare();
    
    // Initialise the default library
    metalDefaultLibrary = metalDevice->newDefaultLibrary();
    
    if(!metalDefaultLibrary)
    {
        std::cerr << "Failed to load default library.";
        std::exit(-1);
    }
    
    //Initialise the command queue
    metalCommandQueue = metalDevice->newCommandQueue();
    
    return true;
}

void CMetalApplication::Run()
{
    while (!glfwWindowShouldClose(glfwWindow))
    {
        pPool = NS::AutoreleasePool::alloc()->init();
        metalDrawable = layer->nextDrawable();
        draw();
        glfwPollEvents();
        pPool->release();
    }
}

void CMetalApplication::Destroy()
{
    glfwTerminate();
    CShaderManager::GetInstance()->Destroy();
    CImageLoader::GetInstance()->Destroy();
    
#ifdef DEBUG
    CEditor::GetInstance()->Destroy();
#endif
    
    metalDevice->release();
    metalCommandQueue->release();

}
void CMetalApplication::frameBufferSizeCallback(GLFWwindow *window, int width, int height)
{
    CMetalApplication* engine = (CMetalApplication*)glfwGetWindowUserPointer(window);
    engine->resizeFrameBuffer(width, height);
}

void CMetalApplication::resizeFrameBuffer(int width, int height)
{
    layer->setDrawableSize(CGSizeMake(width, height));
}

void CMetalApplication::createSquare()
{
    VertexData squareVertices[]
    {
            {{-0.5, -0.5,  0.5, 1.0f}, {0.0f, 0.0f}},
            {{-0.5,  0.5,  0.5, 1.0f}, {0.0f, 1.0f}},
            {{ 0.5,  0.5,  0.5, 1.0f}, {1.0f, 1.0f}},
            {{-0.5, -0.5,  0.5, 1.0f}, {0.0f, 0.0f}},
            {{ 0.5,  0.5,  0.5, 1.0f}, {1.0f, 1.0f}},
            {{ 0.5, -0.5,  0.5, 1.0f}, {1.0f, 0.0f}}
    };

    squareVertexBuffer = metalDevice->newBuffer(&squareVertices, sizeof(squareVertices), MTL::ResourceStorageModeShared);

    CImageLoader::GetInstance()->LoadTexture("assets/mc_grass.png", metalDevice);

}


void CMetalApplication::draw()
{
    metalCommandBuffer = metalCommandQueue->commandBuffer();
    
    renderPassDescriptor = MTL::RenderPassDescriptor::alloc()->init();
    
    MTL::RenderPassColorAttachmentDescriptor* cd = renderPassDescriptor->colorAttachments()->object(0);
    
    cd->setTexture(metalDrawable->texture());
    cd->setLoadAction(MTL::LoadActionClear);
    cd->setClearColor(MTL::ClearColor(CEditor::GetInstance()->GetClearColor(0),
                                      CEditor::GetInstance()->GetClearColor(1),
                                      CEditor::GetInstance()->GetClearColor(2),
                                      CEditor::GetInstance()->GetClearColor(3)));
    cd->setStoreAction(MTL::StoreActionStore);
    
    renderCommandEncoder = metalCommandBuffer->renderCommandEncoder(renderPassDescriptor);
    
    metalRenderPSO = CShaderManager::GetInstance()->getRenderPipelineState("Square");
    renderCommandEncoder->setRenderPipelineState(metalRenderPSO);

    CShaderManager::GetInstance()->BindResources("Square", renderCommandEncoder, squareVertexBuffer);
    renderCommandEncoder->setFragmentTexture(CImageLoader::GetInstance()->GetTexture(), 0);
    renderCommandEncoder->drawPrimitives(MTL::PrimitiveTypeTriangle, NS::UInteger(0), NS::UInteger(6));

#ifdef DEBUG
    CEditor::GetInstance()->Render(renderPassDescriptor, metalCommandBuffer, renderCommandEncoder);
#endif
    
    renderCommandEncoder->endEncoding();

    metalCommandBuffer->presentDrawable(metalDrawable);
    metalCommandBuffer->commit();
    metalCommandBuffer->waitUntilCompleted();

    renderPassDescriptor->release();
}
