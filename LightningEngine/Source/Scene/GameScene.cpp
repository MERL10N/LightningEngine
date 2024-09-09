//
//  GameScene.cpp
//  LightningEngine - macOS
//
//  Created by Kian Marvi on 9/6/24.
//

#include "GameScene.h"

#include <LightningEngineCore/MeshBuilder.h>
#include <LightningEngineCore/Timer.h>
#include <LightningEngineCore/Controller.h>
#include <LightningEngineCore/ImageLoader.h>
#include <CoreGraphics/CoreGraphics.h>
#include <LightningEngineCore/MetalRenderer.h>
#include <LightningEngineCore/MetalKit.hpp>
#include <LightningEngineCore/AAPLMathUtilities.h>
#include <LightningEngineCore/VertexData.h>

void GameScene::Init(MTK::View *view)
{
    camera = Camera();
    fov = camera.GetZoom() * (M_PI / 180.0f);
    nearZ = 0.1f;
    farZ = 100.f;
    
}

void GameScene::Render(MTK::View *view) 
{
    angleInDegrees = Timer::GetTimeInSeconds() * 0.5f * 45.f;
    angleInRadians = angleInDegrees * M_PI / 180.0f;
    
    rotationMatrix = matrix4x4_rotation(angleInRadians, 0.0, 1.0, 0.0);

    modelMatrix = simd_mul(translationMatrix, rotationMatrix);
    viewMatrix = camera.GetViewMatrix();
    
    aspectRatio = (width / height);
    perspectiveMatrix = matrix_perspective_right_hand(fov, aspectRatio, nearZ, farZ);
    
    TransformationData transformationData = { modelMatrix, viewMatrix, perspectiveMatrix };
    
    //renderer->Draw(view, "Shader3D");
    memcpy(transformationBuffer->contents(), &transformationData, sizeof(transformationData));
    
    ProcessInput();
}

void GameScene::Release() 
{
    //imageLoader.Destroy();
    //shaderManager.Destroy();
    //delete renderer;
}

void GameScene::ProcessInput()
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
        camera.ProcessGamepadLeftJoystick(deltaTime, Controller::LeftThumbstickX(), Controller::LeftThumbstickY());
    
    if (Controller::RightThumbstickX() || Controller::RightThumbstickY())
        camera.ProcessGamepadRightJoystick(Controller::RightThumbstickX(), Controller::RightThumbstickY(), true);
     
}

void GameScene::UpdateMousePosition(float &x, float &y)
{
       float xoffset = x - lastX;
       float yoffset = lastY - y; // reversed since y-coordinates go from bottom to top

       lastX = x;
       lastY = y;

       camera.ProcessMouseMovement(xoffset, yoffset, false);
}




