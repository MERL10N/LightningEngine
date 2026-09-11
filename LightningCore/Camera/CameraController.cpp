//
//  CameraController.cpp
//  LightningCore
//
//  Created by Kian Marvi on 8/28/26.
//

#include "CameraController.h"
#include "Input/AppleController.h"
#include "Camera.h"

CameraController::CameraController(const float aspectRatio, const bool rotation)
:  m_Rotation(rotation),
   m_AspectRatio(aspectRatio)
{
   
}

void CameraController::Update(Camera& camera, const float dt)
{
    if (m_AppleController.IsWKeyDown())
    {
        camera.ProcessKeyboardInput(CAMERA_MOVEMENT::FORWARD, dt);
    }
    if (m_AppleController.IsSKeyDown())
    {
        camera.ProcessKeyboardInput(CAMERA_MOVEMENT::BACKWARD, dt);
    }
    if (m_AppleController.IsAKeyDown())
    {
        camera.ProcessKeyboardInput(CAMERA_MOVEMENT::LEFT, dt);
    }
    if (m_AppleController.IsDKeyDown())
    {
        camera.ProcessKeyboardInput(CAMERA_MOVEMENT::RIGHT, dt);
    }
    
    camera.ProcessControllerLeftThumbstickInput(dt, m_AppleController.LeftThumbstickX(), m_AppleController.LeftThumbstickY());

    camera.ProcessControllerRightThumbstickInput(m_AppleController.RightThumbstickX(), m_AppleController.RightThumbstickY());
}
