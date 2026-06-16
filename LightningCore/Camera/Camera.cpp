//
//  Camera.cpp
//  LightningCore
//
//  Created by Kian Marvi on 12/25/25.
//

#include "Camera.h"


Camera::Camera(simd::float3 position, simd::float3 up, float yaw, float pitch)
: m_Position(position),
  m_WorldUp(up),
  m_Yaw(yaw),
  m_Pitch(pitch),
  m_MovementSpeed(SPEED),
  m_MouseSensitivity(SENSITIVITY)
{
    UpdateCameraVectors();
}

Camera::~Camera()
{
}


void Camera::ProcessKeyboardInput(const CAMERA_MOVEMENT &direction, const float deltaTime)
{
    m_Velocity = m_MovementSpeed * deltaTime;
    
    switch (direction)
    {
        case CAMERA_MOVEMENT::FORWARD:
            m_Position += m_Front * m_Velocity;
            break;
        case CAMERA_MOVEMENT::BACKWARD:
            m_Position -= m_Front * m_Velocity;
            break;
        case CAMERA_MOVEMENT::LEFT:
            m_Position -= m_Right * m_Velocity;
            break;
        case CAMERA_MOVEMENT::RIGHT:
            m_Position += m_Right * m_Velocity;
            break;
        case CAMERA_MOVEMENT::UP:
            m_Position += m_Up * m_Velocity;
            break;
        case CAMERA_MOVEMENT::DOWN:
            m_Position -= m_Up * m_Velocity;
            break;
    }
}

void Camera::ProcessControllerLeftThumbstickInput(const float p_DeltaTime, const float p_AxisValueX, const float p_AxisValueY)
{
    m_Velocity = m_MovementSpeed * p_DeltaTime;
    
    m_Position += m_Right * m_Velocity * p_AxisValueX;
    m_Position += m_Front * m_Velocity * p_AxisValueY;
}

void Camera::ProcessControllerRightThumbstickInput(const float p_AxisValueX, const float p_AxisValueY, const bool p_ConstrainPitch)
{
    m_Yaw   += p_AxisValueX;
    m_Pitch += p_AxisValueY;

  if (p_ConstrainPitch)
  {
      if (m_Pitch > 89.0f)
          m_Pitch = 89.0f;
      if (m_Pitch < -89.0f)
          m_Pitch = -89.0f;
  }
    UpdateCameraVectors();
}

void Camera::UpdateCameraVectors()
{
    simd::float3 front;
    front.x = cos(Radians(m_Yaw)) * cos(Radians(m_Pitch));
    front.y = sin(Radians(m_Pitch));
    front.z = sin(Radians(m_Yaw)) * cos(Radians(m_Pitch));
    
    m_Front = simd::normalize(front);
    
    m_Right = simd::normalize(simd::cross(m_Front, m_WorldUp));
    m_Up = simd::normalize(simd::cross(m_Right, m_Front));
}

void Camera::ProcessMouseMovement(float xOffset, float yOffset, const bool constrainPitch)
{
    xOffset *= m_MouseSensitivity;
    yOffset *= m_MouseSensitivity;

    m_Yaw   += xOffset;
    m_Pitch += yOffset;

    // Prevent flipping the screen
    if (constrainPitch)
    {
        if (m_Pitch > 89.0f)
            m_Pitch = 89.0f;
        if (m_Pitch < -89.0f)
            m_Pitch = -89.0f;
    }
    
    UpdateCameraVectors();
}

float Camera::Radians(float degrees)
{
    return degrees * M_PI / 180.0f;
}

simd::float4x4 Camera::LookAt(const simd::float3 &eye, const simd::float3 &center, const simd::float3 &up) const
{
    simd::float3 z = simd::normalize(eye - center);
    simd::float3 x = simd::normalize(simd::cross(up, z));
    simd::float3 y = simd::cross(z, x);

    simd::float4x4 viewMatrix;
    viewMatrix.columns[0] = simd::make_float4(x, 0);
    viewMatrix.columns[1] = simd::make_float4(y, 0);
    viewMatrix.columns[2] = simd::make_float4(z, 0);
    viewMatrix.columns[3] = simd::make_float4(eye, 1);

    return simd::inverse(viewMatrix);
}






