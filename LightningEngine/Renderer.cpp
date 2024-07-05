//
//  Renderer.cpp
//  LightningEngine
//
//  Created by Kian Marvi on 6/26/24.
//

#include "Renderer.h"

//  MetalApplication.cpp
//  Lightning Engine

#include <RenderControl/ShaderManager.h>

#include "VertexData.h"
#include "System/ImageLoader.h"
#include "Math/AAPLMathUtilities.h"

#include <Metal/Metal.hpp>
#include <MetalKit/MetalKit.hpp>
#include "Primitives/MeshBuilder.h"
#include <Editor/Editor.h>

#include <iostream>

CRenderer::CRenderer(MTL::Device* pDevice)
{
    // Detect if metal is supported on the target device
    metalDevice = MTL::CreateSystemDefaultDevice();
    
    
    // Initialise the shader
    CShaderManager::GetInstance()->Add("Square",
                                       "Shaders/square.metal",
                                       "vertexShader",
                                       "fragmentShader",
                                        metalDevice);

    // Render the square
    CreateSquare();
    
    // Initialise the default library
    metalDefaultLibrary = metalDevice->newDefaultLibrary();
    
    if(!metalDefaultLibrary)
    {
        std::cerr << "Failed to load default library.";
        std::exit(-1);
    }
    
    // Initialise the background color
    CEditor::GetInstance()->SetClearColor(0.15f, 0);
    CEditor::GetInstance()->SetClearColor(0.15f, 1);
    CEditor::GetInstance()->SetClearColor(0.15f, 2);
    CEditor::GetInstance()->SetClearColor(1.f, 3);
    
    //Initialise the command queue
    metalCommandQueue = metalDevice->newCommandQueue();
}

CRenderer::~CRenderer()
{
    CShaderManager::GetInstance()->Destroy();
    CImageLoader::GetInstance()->Destroy();

    
    metalDevice->release();
    metalCommandQueue->release();

}

void CRenderer::CreateSquare()
{
    
    squareVertexBuffer = CMeshBuilder::GenerateQuad(metalDevice);
    CImageLoader::GetInstance()->LoadTexture("assets/mc_grass.png", metalDevice);

}


void CRenderer::Draw(MTK::View* view)
{
    autoReleasePool = NS::AutoreleasePool::alloc()->init();
    metalCommandBuffer = metalCommandQueue->commandBuffer();
    
    renderPassDescriptor = view->currentRenderPassDescriptor();

    
    renderCommandEncoder = metalCommandBuffer->renderCommandEncoder(renderPassDescriptor);
    
    metalRenderPSO = CShaderManager::GetInstance()->getRenderPipelineState("Square");
    renderCommandEncoder->setRenderPipelineState(metalRenderPSO);

    CShaderManager::GetInstance()->BindResources("Square", renderCommandEncoder, squareVertexBuffer);
    renderCommandEncoder->setFragmentTexture(CImageLoader::GetInstance()->GetTexture(), 0);
    renderCommandEncoder->drawPrimitives(MTL::PrimitiveTypeTriangle, NS::UInteger(0), NS::UInteger(6));
    
    
    CEditor::GetInstance()->Render(renderPassDescriptor, metalCommandBuffer, renderCommandEncoder, view);
    view->setClearColor( MTL::ClearColor::Make( CEditor::GetInstance()->GetClearColor(0),
                                                     CEditor::GetInstance()->GetClearColor(1),
                                                     CEditor::GetInstance()->GetClearColor(2),
                                                     CEditor::GetInstance()->GetClearColor(3)));
    

    renderCommandEncoder->endEncoding();

    metalCommandBuffer->presentDrawable(view->currentDrawable());
    metalCommandBuffer->commit();
    autoReleasePool->release();
}

