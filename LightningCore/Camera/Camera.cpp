//
//  Camera.cpp
//  LightningCore
//
//  Created by Kian Marvi on 12/25/25.
//

#include "Camera.h"


Camera::Camera(float3 position, float3 up, float yaw, float pitch)
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
    float3 front;
    front.x = cos(Radians(m_Yaw)) * cos(Radians(m_Pitch));
    front.y = sin(Radians(m_Pitch));
    front.z = -sin(Radians(m_Yaw)) * cos(Radians(m_Pitch));
    
    m_Front = normalize(front);
    
    m_Right = normalize(cross(m_WorldUp, m_Front));
    m_Up = normalize(cross(m_Front, m_Right));
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

float Camera::Radians(float degrees) const 
{
    return degrees * M_PI / 180.0f;
}

float4x4 Camera::LookAt(const float3 eye, const float3 center, const float3 up) const
{
    float3 z = normalize(center - eye);
    float3 x = normalize(cross(up, z));
    float3 y = cross(z, x);

    return float4x4(float4(x.x, y.x, z.x, 0.0f),
                    float4(x.y, y.y, z.y, 0.0f),
                    float4(x.z, y.z, z.z, 0.0f),
                    float4(-dot(x,eye), -dot(y,eye), -dot(z,eye), 1.0f));
}






