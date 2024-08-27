/*
      MetalRenderer.cpp
      LightningEngine

      Created by Kian Marvi on 6/26/24.
*/

#include "MetalRenderer.h"

#include <Renderer/ShaderManager.h>

#include "VertexData.h"
#include "System/ImageLoader.h"
#include "Math/AAPLMathUtilities.h"

#include <Metal/Metal.hpp>
#include <MetalKit/MetalKit.hpp>
#include "Primitives/MeshBuilder.h"
#include <Editor/Editor.h>
#include <CoreGraphics/CoreGraphics.h>
#include <TimeControl/Timer.h>
#include <Input/Controller.h>
#include <iostream>

MetalRenderer::MetalRenderer(MTL::Device* metalDevice)
: metalDevice(metalDevice)
, metalDefaultLibrary(metalDevice->newDefaultLibrary())
, metalCommandQueue(metalDevice->newCommandQueue())
, camera(Camera())
, fov(camera.GetZoom() * (M_PI / 180.0f))
, nearZ(0.1f)
, farZ(100.f)
, translationMatrix(matrix4x4_translation(0.f, 0.f,-10.f))
{
    
    if(!metalDefaultLibrary)
    {
        std::cerr << "Failed to load default library.";
        std::exit(-1);
    }
    
    // Initialise the shader
    CShaderManager::GetInstance()->Add("Cube",
                                       "Shaders/cube.metal",
                                       "vertexShader",
                                       "fragmentShader",
                                        metalDevice);
    
    // Render the cube
    CreateCube();
    
}

MetalRenderer::~MetalRenderer()
{
    CShaderManager::GetInstance()->Destroy();
    CImageLoader::GetInstance()->Destroy();
    metalCommandQueue->release();
    squareVertexBuffer->release();
    transformationBuffer->release();
    depthTexture->release();
    msaaRenderTargetTexture->release();
    renderPassDescriptor->release();
    renderCommandEncoder->release();

}

void MetalRenderer::CreateCube()
{
    squareVertexBuffer = CMeshBuilder::GenerateCube(metalDevice);
    transformationBuffer = metalDevice->newBuffer(sizeof(TransformationData), MTL::ResourceStorageModeShared);
    CImageLoader::GetInstance()->LoadTexture("assets/mc_grass.png", metalDevice);
}


void MetalRenderer::Draw(MTK::View* view)
{
    metalCommandBuffer = metalCommandQueue->commandBuffer();
    renderPassDescriptor = view->currentRenderPassDescriptor();

    CGSize drawableSize = view->drawableSize();
    
    // Only recreate texture when the drawable size has changed. Otherwise reuse old textures to save CPU performance.
    if (drawableSize.width != width || drawableSize.height != height)
    {
        CreateDepthAndMSAATextures(view, drawableSize);
    }
    
#ifdef DEBUG
    view->setPreferredFramesPerSecond(60);
#else
    view->setPreferredFramesPerSecond(120);
#endif
    
    colorAttachmentDescriptor = renderPassDescriptor->colorAttachments()->object(0);

    
    colorAttachmentDescriptor->setTexture(msaaRenderTargetTexture);
    colorAttachmentDescriptor->setResolveTexture(view->currentDrawable()->texture());
    colorAttachmentDescriptor->setLoadAction(MTL::LoadActionClear);
    
#ifdef DEBUG
    colorAttachmentDescriptor->setClearColor(MTL::ClearColor(CEditor::GetInstance()->GetClearColor(0),
                             CEditor::GetInstance()->GetClearColor(1),
                             CEditor::GetInstance()->GetClearColor(2),
                             CEditor::GetInstance()->GetClearColor(3)));
#else
    colorAttachmentDescriptor->setClearColor(MTL::ClearColor(MTL::ClearColor(0.15f, 0.15f, 0.15f, 0.15f)));
#endif
    
    colorAttachmentDescriptor->setStoreAction(MTL::StoreActionMultisampleResolve);

    depthAttachment = renderPassDescriptor->depthAttachment();
    depthAttachment->setTexture(depthTexture);
    depthAttachment->setLoadAction(MTL::LoadActionClear);
    depthAttachment->setStoreAction(MTL::StoreActionDontCare);
    depthAttachment->setClearDepth(1.0);

    renderCommandEncoder = metalCommandBuffer->renderCommandEncoder(renderPassDescriptor);

    renderPassDescriptor->colorAttachments()->object(0)->setTexture(msaaRenderTargetTexture);
    renderPassDescriptor->colorAttachments()->object(0)->setResolveTexture(view->currentDrawable()->texture());
    renderPassDescriptor->depthAttachment()->setTexture(depthTexture);


    angleInDegrees = Timer::GetTimeInSeconds() * 0.5f * 45;
    angleInRadians = angleInDegrees * M_PI / 180.0f;
    
    rotationMatrix = matrix4x4_rotation(angleInRadians, 0.0, 1.0, 0.0);

    modelMatrix = simd_mul(translationMatrix, rotationMatrix);
    viewMatrix = camera.GetViewMatrix();
    
    aspectRatio = (drawableSize.width / drawableSize.height);
    perspectiveMatrix = matrix_perspective_right_hand(fov, aspectRatio, nearZ, farZ);

    TransformationData transformationData = { modelMatrix, viewMatrix, perspectiveMatrix };
    memcpy(transformationBuffer->contents(), &transformationData, sizeof(transformationData));


    metalRenderPSO = CShaderManager::GetInstance()->GetRenderPipelineState("Cube");
    renderCommandEncoder->setFrontFacingWinding(MTL::WindingCounterClockwise);
    renderCommandEncoder->setCullMode(MTL::CullModeBack); // Enable backface culling
    renderCommandEncoder->setRenderPipelineState(metalRenderPSO);
    renderCommandEncoder->setDepthStencilState(CShaderManager::GetInstance()->getDepthStencilState("Cube"));
    CShaderManager::GetInstance()->BindResources("Cube", renderCommandEncoder, squareVertexBuffer);
    renderCommandEncoder->setFragmentTexture(CImageLoader::GetInstance()->GetTexture(), 0);
    renderCommandEncoder->setVertexBuffer(transformationBuffer, 0, 1);
    renderCommandEncoder->setVertexBytes(&viewMatrix, sizeof(viewMatrix), 2);
    renderCommandEncoder->setVertexBytes(&perspectiveMatrix, sizeof(perspectiveMatrix), 3);
    renderCommandEncoder->drawPrimitives(MTL::PrimitiveTypeTriangle, NS::UInteger(0), NS::UInteger(36));
    
    #ifdef DEBUG
    CEditor::GetInstance()->Render(renderPassDescriptor, metalCommandBuffer, renderCommandEncoder, view);
    #endif
    
    ProcessInput();

    renderCommandEncoder->endEncoding();

    metalCommandBuffer->presentDrawable(view->currentDrawable());
    metalCommandBuffer->commit();
}

void MetalRenderer::CreateDepthAndMSAATextures(MTK::View* view, CGSize &size)
{
    width = size.width;
    height = size.height;
    
    // Deallocate the already existing target textures to avoid memory leak
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
    msaaTextureDescriptor->setStorageMode(MTL::StorageModePrivate);
    msaaTextureDescriptor->setUsage(MTL::TextureUsageRenderTarget);

    msaaRenderTargetTexture = metalDevice->newTexture(msaaTextureDescriptor);

    MTL::TextureDescriptor* depthTextureDescriptor = MTL::TextureDescriptor::alloc()->init();
    depthTextureDescriptor->setTextureType(MTL::TextureType2DMultisample);
    depthTextureDescriptor->setPixelFormat(MTL::PixelFormatDepth32Float);
    depthTextureDescriptor->setWidth(width);
    depthTextureDescriptor->setHeight(height);
    depthTextureDescriptor->setSampleCount(4);
    depthTextureDescriptor->setStorageMode(MTL::StorageModePrivate);
    depthTextureDescriptor->setUsage(MTL::TextureUsageRenderTarget);


    depthTexture = metalDevice->newTexture(depthTextureDescriptor);

    msaaTextureDescriptor->release();
    depthTextureDescriptor->release();
}

void MetalRenderer::ProcessInput() 
{
    currentFrame = Timer::GetTimeInSeconds();
    deltaTime = currentFrame - lastFrame;
    lastFrame = currentFrame;
    
    Controller gameController;
    
    if (gameController.isRightMouseClicked())
    {
        mouseX = gameController.getMousePosition().x;
        mouseY = gameController.getMousePosition().y;
        
        if (firstMouse)
        {
               lastX = mouseX;
               lastY = mouseY;
               firstMouse = false;
        }
        else
        {
            UpdateMousePosition(mouseX, mouseY);
        }
        gameController.hideCursor();
    }
    else
    {
        gameController.showCursor();
        firstMouse = true;
    }
    
        
    if (gameController.isWKeyDown())
        camera.ProcessKeyboard(FORWARD, deltaTime);
        
    if (gameController.isAKeyDown())
        camera.ProcessKeyboard(LEFT, deltaTime);
        
    if (gameController.isSKeyDown())
        camera.ProcessKeyboard(BACKWARD, deltaTime);
        
    if (gameController.isDKeyDown())
        camera.ProcessKeyboard(RIGHT, deltaTime);
     
    if (gameController.leftThumbstickX() || gameController.leftThumbstickY())
        camera.ProcessGamepadLeftJoystick(deltaTime, gameController.leftThumbstickX(), gameController.leftThumbstickY());
    
    if (gameController.rightThumbstickX() || gameController.rightThumbstickY())
        camera.ProcessGamepadRightJoystick(gameController.rightThumbstickX(), gameController.rightThumbstickY(), true);
     
}

void MetalRenderer::UpdateMousePosition(float &x, float &y) 
{
       float xoffset = x - lastX;
       float yoffset = lastY - y; // reversed since y-coordinates go from bottom to top

       lastX = x;
       lastY = y;

       camera.ProcessMouseMovement(xoffset, yoffset, false);
}


