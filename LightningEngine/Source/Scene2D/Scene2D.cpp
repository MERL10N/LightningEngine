//
//  Scene2D.cpp
//  LightningEngine
//
//  Created by Kian Marvi on 2/2/24.
//

#include "Scene2D.h"
#include <RenderControl/ShaderManager.h>
#include <System/ImageLoader.h>

#include <Metal/Metal.hpp>
#include <QuartzCore/CAMetalLayer.hpp>
#include <QuartzCore/QuartzCore.hpp>

#include "VertexData.h"
#include "Editor.h"
#include <iostream>

CScene2D::CScene2D(MTL::Device* metalDevice)
{
    
    this->metalDevice = metalDevice;
    
    // Detect if metal is supported on the target device
    metalDevice = MTL::CreateSystemDefaultDevice();
    
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
}

CScene2D::~CScene2D()
{
    CShaderManager::GetInstance()->Destroy();
    CImageLoader::GetInstance()->Destroy();
    metalDevice->release();
    metalCommandQueue->release();
}

void CScene2D::createSquare()
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


void CScene2D::Render(CA::MetalDrawable* metalDrawable)
{
    pPool = NS::AutoreleasePool::alloc()->init();
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
    
    renderCommandEncoder->endEncoding();

    metalCommandBuffer->presentDrawable(metalDrawable);
    metalCommandBuffer->commit();
    metalCommandBuffer->waitUntilCompleted();

    renderPassDescriptor->release();
    pPool->release();
}
