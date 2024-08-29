/*
      MetalRenderer.cpp
      LightningEngine

      Created by Kian Marvi on 6/26/24.
*/

#include "MetalRenderer.h"

#include <Renderer/ShaderManager.h>

#include "VertexData.h"
#include "Math/AAPLMathUtilities.h"

#include <Metal/Metal.hpp>
#include <MetalKit/MetalKit.hpp>
#include "Primitives/MeshBuilder.h"
#include <CoreGraphics/CoreGraphics.h>
#include <TimeControl/Timer.h>
#include <Input/Controller.h>
#include <System/ImageLoader.h>
#include <iostream>

CShaderManager shaderManager;
CImageLoader imageLoader;

MetalRenderer::MetalRenderer(MTL::Device* metalDevice, MTK::View* view)
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
     shaderManager.Add("Cube",
                       "Shaders/cube.metal",
                        metalDevice, view);
    imageLoader.Init();
    
    
    // Render the cube
    CreateCube();
}

MetalRenderer::~MetalRenderer()
{
    shaderManager.Destroy();
    
    imageLoader.Destroy();
    
    metalCommandQueue->release();
    cubeVertexBuffer->release();
    transformationBuffer->release();
    renderPassDescriptor->release();
    renderCommandEncoder->release();
}

void MetalRenderer::Init(MTL::Device* metalDevice, MTK::View* view)
{
    this->metalDevice = metalDevice;
    metalDefaultLibrary = metalDevice->newDefaultLibrary();
    metalCommandQueue = metalDevice->newCommandQueue();
    camera = Camera();
    
    fov = camera.GetZoom() * (M_PI / 180.0f);
    nearZ = 0.1f;
    farZ = 100.f;
    
    translationMatrix = matrix4x4_translation(0.f, 0.f,-10.f);
    
    if(!metalDefaultLibrary)
    {
        std::cerr << "Failed to load default library.";
        std::exit(-1);
    }
    
    // Initialise the shader
     shaderManager.Add("Cube",
                       "Shaders/cube.metal",
                        metalDevice, view);
    
    imageLoader.Init();
    
#ifdef DEBUG
    offscreenTextureDescriptor = MTL::TextureDescriptor::alloc()->init();
    offscreenTextureDescriptor->setTextureType(MTL::TextureType2D);
    offscreenTextureDescriptor->setUsage(MTL::TextureUsageRenderTarget | MTL::TextureUsageShaderRead);
    offscreenTextureDescriptor->setWidth(width);
    offscreenTextureDescriptor->setHeight(height);
    offScreenTexture = metalDevice->newTexture(offscreenTextureDescriptor);
#endif
    
    // Render the cube
    CreateCube();
}

void MetalRenderer::Destroy()
{
    shaderManager.Destroy();
    imageLoader.Destroy();
    metalCommandQueue->release();
    cubeVertexBuffer->release();
    transformationBuffer->release();
    renderPassDescriptor->release();
    renderCommandEncoder->release();
    offScreenTexture->release();
    offscreenTextureDescriptor->release();
}

void MetalRenderer::CreateCube()
{
    cubeVertexBuffer = MeshBuilder::GenerateCube();
    transformationBuffer = metalDevice->newBuffer(sizeof(TransformationData), MTL::ResourceStorageModeShared);
    imageLoader.LoadTexture("assets/mc_grass.png");
}


void MetalRenderer::Draw(MTK::View* view)
{
    metalCommandBuffer = metalCommandQueue->commandBuffer();
    renderPassDescriptor = view->currentRenderPassDescriptor();

    CGSize drawableSize = view->drawableSize();
    
    // Only recreate texture when the drawable size has changed. Otherwise reuse old textures to save CPU performance.
    if (drawableSize.width != width || drawableSize.height != height)
    {
        imageLoader.CreateDepthAndMSAATextures(width, height, drawableSize);
    }
    
    view->setPreferredFramesPerSecond(120);
    
    
    colorAttachmentDescriptor = renderPassDescriptor->colorAttachments()->object(0);

    
    colorAttachmentDescriptor->setTexture(imageLoader.GetTargetTexture());
    colorAttachmentDescriptor->setResolveTexture(view->currentDrawable()->texture());
    colorAttachmentDescriptor->setLoadAction(MTL::LoadActionClear);
    
#ifdef DEBUG
    colorAttachmentDescriptor->setClearColor(MTL::ClearColor(editor.GetClearColor(0),
                                                             editor.GetClearColor(1),
                                                             editor.GetClearColor(2),
                                                             editor.GetClearColor(3)));
#else
    colorAttachmentDescriptor->setClearColor(MTL::ClearColor(MTL::ClearColor(0.15f, 0.15f, 0.15f, 0.15f)));
#endif
    
    colorAttachmentDescriptor->setStoreAction(MTL::StoreActionMultisampleResolve);

    depthAttachment = renderPassDescriptor->depthAttachment();
    depthAttachment->setTexture(imageLoader.GetDepthTexture());
    depthAttachment->setLoadAction(MTL::LoadActionClear);
    depthAttachment->setStoreAction(MTL::StoreActionDontCare);
    depthAttachment->setClearDepth(1.0);

    renderCommandEncoder = metalCommandBuffer->renderCommandEncoder(renderPassDescriptor);

    renderPassDescriptor->colorAttachments()->object(0)->setTexture(imageLoader.GetTargetTexture());
    renderPassDescriptor->colorAttachments()->object(0)->setResolveTexture(view->currentDrawable()->texture());
    
#ifdef DEBUG
    renderPassDescriptor->colorAttachments()->object(1)->setTexture(offScreenTexture);
    renderPassDescriptor->colorAttachments()->object(1)->setLoadAction(MTL::LoadActionClear);
    renderPassDescriptor->colorAttachments()->object(1)->setClearColor(MTL::ClearColor::Make(0.0, 0.0, 0.0, 1.0));
    renderPassDescriptor->colorAttachments()->object(1)->setStoreAction(MTL::StoreActionStore);
#endif

    renderPassDescriptor->depthAttachment()->setTexture(imageLoader.GetDepthTexture());

    angleInDegrees = Timer::GetTimeInSeconds() * 0.5f * 45.f;
    angleInRadians = angleInDegrees * M_PI / 180.0f;
    
    rotationMatrix = matrix4x4_rotation(angleInRadians, 0.0, 1.0, 0.0);

    modelMatrix = simd_mul(translationMatrix, rotationMatrix);
    viewMatrix = camera.GetViewMatrix();
    
    aspectRatio = (drawableSize.width / drawableSize.height);
    perspectiveMatrix = matrix_perspective_right_hand(fov, aspectRatio, nearZ, farZ);

    TransformationData transformationData = { modelMatrix, viewMatrix, perspectiveMatrix };
    memcpy(transformationBuffer->contents(), &transformationData, sizeof(transformationData));


    metalRenderPSO = shaderManager.GetRenderPipelineState("Cube");
    renderCommandEncoder->setFrontFacingWinding(MTL::WindingCounterClockwise);
    renderCommandEncoder->setCullMode(MTL::CullModeBack); // Enable backface culling
    renderCommandEncoder->setRenderPipelineState(metalRenderPSO);
    renderCommandEncoder->setDepthStencilState(shaderManager.GetDepthStencilState("Cube"));
    shaderManager.BindResources("Cube", renderCommandEncoder, cubeVertexBuffer);
    renderCommandEncoder->setFragmentTexture(imageLoader.GetTexture(), 0);
    renderCommandEncoder->setVertexBuffer(transformationBuffer, 0, 1);
    renderCommandEncoder->setVertexBytes(&viewMatrix, sizeof(viewMatrix), 2);
    renderCommandEncoder->setVertexBytes(&perspectiveMatrix, sizeof(perspectiveMatrix), 3);
    renderCommandEncoder->drawPrimitives(MTL::PrimitiveTypeTriangle, NS::UInteger(0), NS::UInteger(36));
    
    #ifdef DEBUG
    editor.Render(renderPassDescriptor, metalCommandBuffer, renderCommandEncoder, view, offScreenTexture);
    #endif
    
    ProcessInput();

    renderCommandEncoder->endEncoding();

    metalCommandBuffer->presentDrawable(view->currentDrawable());
    metalCommandBuffer->commit();
}

void MetalRenderer::ProcessInput() 
{
    currentFrame = Timer::GetTimeInSeconds();
    deltaTime = currentFrame - lastFrame;
    lastFrame = currentFrame;
    
    if (Controller::isRightMouseClicked())
    {
        mouseX = Controller::getMousePosition().x;
        mouseY = Controller::getMousePosition().y;
        
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
        Controller::hideCursor();
    }
    else
    {
        Controller::showCursor();
        firstMouse = true;
    }
    
        
    if (Controller::isWKeyDown())
        camera.ProcessKeyboard(FORWARD, deltaTime);
        
    if (Controller::isAKeyDown())
        camera.ProcessKeyboard(LEFT, deltaTime);
        
    if (Controller::isSKeyDown())
        camera.ProcessKeyboard(BACKWARD, deltaTime);
        
    if (Controller::isDKeyDown())
        camera.ProcessKeyboard(RIGHT, deltaTime);
     
    if (Controller::leftThumbstickX() || Controller::leftThumbstickY())
        camera.ProcessGamepadLeftJoystick(deltaTime, Controller::leftThumbstickX(), Controller::leftThumbstickX());
    
    if (Controller::rightThumbstickX() || Controller::rightThumbstickY())
        camera.ProcessGamepadRightJoystick(Controller::rightThumbstickX(), Controller::rightThumbstickY(), true);
     
}

void MetalRenderer::UpdateMousePosition(float &x, float &y) 
{
       float xoffset = x - lastX;
       float yoffset = lastY - y; // reversed since y-coordinates go from bottom to top

       lastX = x;
       lastY = y;

       camera.ProcessMouseMovement(xoffset, yoffset, false);
}

