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
#include <TimeControl/Timer.h>
#include <Input/Controller.h>
#include <System/ImageLoader.h>
#include "MetalFrameBuffer.h"
#include <iostream>

static CShaderManager shaderManager;
static ImageLoader imageLoader;
MetalFrameBuffer frameBuffer;

MetalRenderer::MetalRenderer(MTK::View* view)
: view(view)
, metalDefaultLibrary(view->device()->newDefaultLibrary())
, metalCommandQueue(view->device()->newCommandQueue())
, camera(Camera())
, fov(camera.GetZoom() * (M_PI / 180.0f))
, nearZ(0.1f)
, farZ(100.f)
, translationMatrix(matrix4x4_translation(0.f, 0.f,-10.f))
, drawableSize(view->drawableSize())
{
    if(!metalDefaultLibrary)
    {
        std::cerr << "Failed to load default library.";
        std::exit(-1);
    }
    width = drawableSize.width;
    height = drawableSize.height;
    // Initialise the shader
    shaderManager.Add("Shader3D", "Shaders/Shader3D.metal", view);
    imageLoader.Init();
    frameBuffer.Init(width, height, view->device(), 4);
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

void MetalRenderer::Init(MTK::View* view)
{
    metalDefaultLibrary = view->device()->newDefaultLibrary();
    metalCommandQueue = view->device()->newCommandQueue();
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
                        view);
    
    imageLoader.Init();
    
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
    frameBuffer.Release();
}

void MetalRenderer::CreateCube()
{
    cubeVertexBuffer = MeshBuilder::GenerateCube(view->device());
    transformationBuffer = view->device()->newBuffer(sizeof(TransformationData), MTL::ResourceStorageModeShared);
    imageLoader.LoadTexture("assets/mc_grass.png", view->device());
}


void MetalRenderer::Draw(MTK::View* view)
{
    metalCommandBuffer = metalCommandQueue->commandBuffer();
    renderPassDescriptor = view->currentRenderPassDescriptor();

    drawableSize = view->drawableSize();
    
    if (width != drawableSize.width || height != drawableSize.height)
    {
        frameBuffer.ResizeFrameBuffer(width, height, drawableSize, view->device(), 4);
    }
    
    view->setPreferredFramesPerSecond(120);
    
    depthAttachment = renderPassDescriptor->depthAttachment();
    depthAttachment->setTexture(frameBuffer.GetDepthTexture());
    depthAttachment->setLoadAction(MTL::LoadActionClear);
    depthAttachment->setStoreAction(MTL::StoreActionDontCare);
    depthAttachment->setClearDepth(1.0);

    renderCommandEncoder = metalCommandBuffer->renderCommandEncoder(renderPassDescriptor);

    renderPassDescriptor->colorAttachments()->object(0)->setTexture(frameBuffer.GetTargetTexture());
    renderPassDescriptor->colorAttachments()->object(0)->setResolveTexture(frameBuffer.GetResolvedTexture());
    renderPassDescriptor->colorAttachments()->object(0)->setStoreAction(MTL::StoreActionStoreAndMultisampleResolve);
    renderPassDescriptor->colorAttachments()->object(0)->setLoadAction(MTL::LoadActionLoad);
    renderPassDescriptor->colorAttachments()->object(0)->setClearColor(MTL::ClearColor(view->clearColor().red, view->clearColor().blue, view->clearColor().green, view->clearColor().alpha));
    renderPassDescriptor->depthAttachment()->setTexture(frameBuffer.GetDepthTexture());
    

    angleInDegrees = Timer::GetTimeInSeconds() * 0.5f * 45.f;
    angleInRadians = angleInDegrees * M_PI / 180.0f;
    
    rotationMatrix = matrix4x4_rotation(angleInRadians, 0.0, 1.0, 0.0);

    modelMatrix = simd_mul(translationMatrix, rotationMatrix);
    viewMatrix = camera.GetViewMatrix();
    
    aspectRatio = (width / height);
    perspectiveMatrix = matrix_perspective_right_hand(fov, aspectRatio, nearZ, farZ);

    TransformationData transformationData = { modelMatrix, viewMatrix, perspectiveMatrix };
    memcpy(transformationBuffer->contents(), &transformationData, sizeof(transformationData));

    metalRenderPSO = shaderManager.GetRenderPipelineState("Shader3D");
    renderCommandEncoder->setFrontFacingWinding(MTL::WindingCounterClockwise);
    renderCommandEncoder->setCullMode(MTL::CullModeBack); // Enable backface culling
    renderCommandEncoder->setRenderPipelineState(metalRenderPSO);
    renderCommandEncoder->setDepthStencilState(shaderManager.GetDepthStencilState("Shader3D"));
    shaderManager.BindResources("Shader3D", renderCommandEncoder, cubeVertexBuffer);
    renderCommandEncoder->setFragmentTexture(imageLoader.GetTexture(), 0);
    renderCommandEncoder->setVertexBuffer(transformationBuffer, 0, 1);
    renderCommandEncoder->setVertexBytes(&viewMatrix, sizeof(viewMatrix), 2);
    renderCommandEncoder->setVertexBytes(&perspectiveMatrix, sizeof(perspectiveMatrix), 3);
    renderCommandEncoder->drawPrimitives(MTL::PrimitiveTypeTriangle, NS::UInteger(0), NS::UInteger(36));

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
    
    if (Controller::IsRightMouseClicked())
    {
        mouseX = Controller::GetMousePosition().x;
        mouseY = Controller::GetMousePosition().y;
        
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
        Controller::HideCursor();
    }
    else
    {
        Controller::ShowCursor();
        firstMouse = true;
    }
    
    if (Controller::IsWKeyDown())
        camera.ProcessKeyboard(FORWARD, deltaTime);
        
    if (Controller::IsAKeyDown())
        camera.ProcessKeyboard(LEFT, deltaTime);
        
    if (Controller::IsSKeyDown())
        camera.ProcessKeyboard(BACKWARD, deltaTime);
        
    if (Controller::IsDKeyDown())
        camera.ProcessKeyboard(RIGHT, deltaTime);
     
    if (Controller::LeftThumbstickX() || Controller::LeftThumbstickY())
        camera.ProcessGamepadLeftJoystick(deltaTime, Controller::LeftThumbstickX(), Controller::LeftThumbstickX());
    
    if (Controller::RightThumbstickX() || Controller::RightThumbstickY())
        camera.ProcessGamepadRightJoystick(Controller::RightThumbstickX(), Controller::RightThumbstickY(), true);
     
}

void MetalRenderer::UpdateMousePosition(float &x, float &y) 
{
       float xoffset = x - lastX;
       float yoffset = lastY - y; // reversed since y-coordinates go from bottom to top

       lastX = x;
       lastY = y;

       camera.ProcessMouseMovement(xoffset, yoffset, false);
}
