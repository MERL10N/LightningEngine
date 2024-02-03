//  mtl_engine.mm
//  MetalTutorial

#include "MetalApplication.h"
#include <RenderControl/ShaderManager.h>
#include "Source/Editor/CEditor.h"
#include <QuartzCore/CAMetalLayer.h>
#include <Metal/Metal.h>

#include "VertexData.hpp"
#include "System/ImageLoader.h"
#include "Math/AAPLMathUtilities.h"

#include <simd/simd.h>

#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#define GLFW_EXPOSE_NATIVE_COCOA
#include <GLFW/glfw3native.h>


bool CMetalApplication::init()
{
    dt = 0.f;
    // Detect if metal is supported on the target device
    metalDevice = MTL::CreateSystemDefaultDevice();
    
    // Initialise GLFW
    glfwInit();
    glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
    glfwWindow = glfwCreateWindow(1280, 720, "Metal Engine", nullptr, nullptr);
    
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
    
    metalWindow = glfwGetCocoaWindow(glfwWindow);
    
    layer = CA::MetalLayer::layer();
    layer->setDevice(metalDevice);
    layer->setPixelFormat(MTL::PixelFormatBGRA8Unorm);
    layer->setDrawableSize(CGSizeMake(width, height));
    metalLayer = (__bridge CAMetalLayer*) layer;
    metalWindow.contentView.layer = metalLayer;
    metalWindow.contentView.wantsLayer = YES;
    

    // Initialise the shader
    CShaderManager::GetInstance()->Add("Square",
                                       "LightningEngine/Shaders/square.metal",
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
        draw();
        pPool->release();
        glfwPollEvents();
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
    metalLayer.drawableSize = CGSizeMake(width, height);
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
    metalDrawable = (__bridge CA::MetalDrawable*)[metalLayer nextDrawable];
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

    renderCommandEncoder->setVertexBuffer(squareVertexBuffer, 0, 0);
    renderCommandEncoder->useResource(squareVertexBuffer, MTL::ResourceUsageRead);
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
