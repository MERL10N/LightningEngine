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
#include <CoreGraphics/CoreGraphics.h>
#include <TimeControl/Timer.h>

#include <iostream>

CRenderer::CRenderer(MTL::Device* pDevice)
{
    // Detect if metal is supported on the target device
    metalDevice = MTL::CreateSystemDefaultDevice();
    
    
    // Initialise the shader
    CShaderManager::GetInstance()->Add("Cube",
                                       "Shaders/cube.metal",
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

    #ifdef DEBUG
    CEditor::GetInstance()->Destroy();
    #endif

    metalDevice->release();
    metalCommandQueue->release();
    squareVertexBuffer->release();
    transformationBuffer->release();
    depthTexture->release();
    msaaRenderTargetTexture->release();
    renderPassDescriptor->release();

}

void CRenderer::CreateSquare()
{
    squareVertexBuffer = CMeshBuilder::GenerateCube(metalDevice);
    transformationBuffer = metalDevice->newBuffer(sizeof(TransformationData), MTL::ResourceStorageModeShared);
    CImageLoader::GetInstance()->LoadTexture("assets/mc_grass.png", metalDevice);
}


void CRenderer::Draw(MTK::View* view)
{
    
    metalCommandBuffer = metalCommandQueue->commandBuffer();

    renderPassDescriptor = view->currentRenderPassDescriptor();

    CreateDepthAndMSAATextures(view);

    MTL::RenderPassColorAttachmentDescriptor* colorAttachmentDescriptor = renderPassDescriptor->colorAttachments()->object(0);
    MTL::RenderPassDepthAttachmentDescriptor* depthAttachment = renderPassDescriptor->depthAttachment();

    colorAttachmentDescriptor->setTexture(msaaRenderTargetTexture);
    colorAttachmentDescriptor->setResolveTexture(view->currentDrawable()->texture());
    colorAttachmentDescriptor->setLoadAction(MTL::LoadActionClear);
    colorAttachmentDescriptor->setClearColor(MTL::ClearColor(CEditor::GetInstance()->GetClearColor(0),
                             CEditor::GetInstance()->GetClearColor(1),
                             CEditor::GetInstance()->GetClearColor(2),
                             CEditor::GetInstance()->GetClearColor(3)));
    colorAttachmentDescriptor->setStoreAction(MTL::StoreActionMultisampleResolve);

    depthAttachment->setTexture(depthTexture);
    depthAttachment->setLoadAction(MTL::LoadActionClear);
    depthAttachment->setStoreAction(MTL::StoreActionDontCare);
    depthAttachment->setClearDepth(1.0);

    renderCommandEncoder = metalCommandBuffer->renderCommandEncoder(renderPassDescriptor);

    renderPassDescriptor->colorAttachments()->object(0)->setTexture(msaaRenderTargetTexture);
    renderPassDescriptor->colorAttachments()->object(0)->setResolveTexture(view->currentDrawable()->texture());
    renderPassDescriptor->depthAttachment()->setTexture(depthTexture);

    // Moves the Cube 2 units down the negative Z-axis
    matrix_float4x4 translationMatrix = matrix4x4_translation(0, 0,-1.0);

    float angleInDegrees = Timer::GetTimeInSeconds() * 0.5f * 45;
    float angleInRadians = angleInDegrees * M_PI / 180.0f;
    matrix_float4x4 rotationMatrix = matrix4x4_rotation(angleInRadians, 0.0, 1.0, 0.0);

    matrix_float4x4 modelMatrix = simd_mul(translationMatrix, rotationMatrix);

    simd::float3 R = simd::float3 {1, 0, 0}; // Unit-Right
    simd::float3 U = simd::float3 {0, 1, 0}; // Unit-Up
    simd::float3 F = simd::float3 {0, 0,-1}; // Unit-Forward
    simd::float3 P = simd::float3 {0, 0, 1}; // Camera Position in World Space

    matrix_float4x4 viewMatrix = matrix_make_rows(R.x, R.y, R.z, simd::dot(-R, P),
                                         U.x, U.y, U.z, simd::dot(-U, P),
                                        -F.x,-F.y,-F.z, simd::dot( F, P),
                                         0, 0, 0, 1);

    float aspectRatio = GetAspectRatio();
    float fov = 90 * (M_PI / 180.0f);
    float nearZ = 0.1f;
    float farZ = 100.0f;

    matrix_float4x4 perspectiveMatrix = matrix_perspective_right_hand(fov, aspectRatio, nearZ, farZ);

    TransformationData transformationData = { modelMatrix, viewMatrix, perspectiveMatrix };
    memcpy(transformationBuffer->contents(), &transformationData, sizeof(transformationData));


    metalRenderPSO = CShaderManager::GetInstance()->getRenderPipelineState("Cube");
    renderCommandEncoder->setFrontFacingWinding(MTL::WindingCounterClockwise);
    renderCommandEncoder->setCullMode(MTL::CullModeBack);
    renderCommandEncoder->setRenderPipelineState(metalRenderPSO);
    renderCommandEncoder->setDepthStencilState(CShaderManager::GetInstance()->getDepthStencilState("Cube"));
    CShaderManager::GetInstance()->BindResources("Cube", renderCommandEncoder, squareVertexBuffer);
    renderCommandEncoder->setFragmentTexture(CImageLoader::GetInstance()->GetTexture(), 0);
    renderCommandEncoder->setVertexBuffer(transformationBuffer, 0, 1);
    renderCommandEncoder->drawPrimitives(MTL::PrimitiveTypeTriangle, NS::UInteger(0), NS::UInteger(36));

    #ifdef DEBUG
    CEditor::GetInstance()->Render(renderPassDescriptor, metalCommandBuffer, renderCommandEncoder, view);
    #endif

    renderCommandEncoder->endEncoding();

    metalCommandBuffer->presentDrawable(view->currentDrawable());
    metalCommandBuffer->commit();
    metalCommandBuffer->waitUntilCompleted();
}

void CRenderer::CreateDepthAndMSAATextures(MTK::View* view)
{
    CGSize drawableSize = view->drawableSize();
    width = drawableSize.width;
    height = drawableSize.height;
    
    if (msaaRenderTargetTexture)
    {
        msaaRenderTargetTexture->release();
    }
    
    if (depthTexture)
    {
        depthTexture->release();
    }

    MTL::TextureDescriptor* msaaTextureDescriptor = MTL::TextureDescriptor::alloc()->init();
    msaaTextureDescriptor->setTextureType(MTL::TextureType2DMultisample);
    msaaTextureDescriptor->setPixelFormat(MTL::PixelFormatBGRA8Unorm);
    msaaTextureDescriptor->setWidth(width);
    msaaTextureDescriptor->setHeight(height);
    msaaTextureDescriptor->setSampleCount(4);
    msaaTextureDescriptor->setUsage(MTL::TextureUsageRenderTarget);

    msaaRenderTargetTexture = metalDevice->newTexture(msaaTextureDescriptor);

    MTL::TextureDescriptor* depthTextureDescriptor = MTL::TextureDescriptor::alloc()->init();
    depthTextureDescriptor->setTextureType(MTL::TextureType2DMultisample);
    depthTextureDescriptor->setPixelFormat(MTL::PixelFormatDepth32Float);
    depthTextureDescriptor->setWidth(width);
    depthTextureDescriptor->setHeight(height);
    depthTextureDescriptor->setUsage(MTL::TextureUsageRenderTarget);
    depthTextureDescriptor->setSampleCount(4);

    depthTexture = metalDevice->newTexture(depthTextureDescriptor);

    msaaTextureDescriptor->release();
    depthTextureDescriptor->release();
}

inline float CRenderer::GetAspectRatio()
{
    return width / height;
}
