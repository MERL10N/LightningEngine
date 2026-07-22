//
//  Camera.h
//  LightningCore
//
//  Created by Kian Marvi on 12/25/25.
//

#ifndef Camera_h
#define Camera_h

#define HLSLPP_FEATURE_TRANSFORM
#include <hlsl++.h>

using namespace hlslpp;

enum class CAMERA_MOVEMENT
{
    FORWARD,
    BACKWARD,
    LEFT,
    RIGHT,
    UP,
    DOWN
};

constexpr float YAW         = -90.0f;
constexpr float PITCH       = 0.0f;
constexpr float SPEED       = 2.5f;
constexpr float SENSITIVITY = 0.1f;
constexpr float ZOOM        = 60.0f;

class Camera
{
public:
    Camera(float3 position = float3(0.0f, 0.0f, -5.0f), float3 up = float3(0.0f, 1.0f, 0.0f), float yaw = YAW, float pitch = PITCH);
    ~Camera();
    
    void ProcessKeyboardInput(const CAMERA_MOVEMENT &direction, const float deltaTime);
    void ProcessControllerLeftThumbstickInput(const float p_DeltaTime, const float p_AxisValueX, const float p_AxisValueY);
    void ProcessControllerRightThumbstickInput(const float p_AxisValueX, const float p_AxisValueY, const bool p_ConstrainPitch = true);
    void ProcessMouseMovement(float xOffset, const float yOffset, const bool constrainPitch);
    
    inline float4x4 GetViewMatrix() const { return LookAt(m_Position, m_Position + m_Front, m_Up); }
    
    inline float GetZoom() const { return m_Zoom; }
    
    inline float3 GetPosition() const  { return m_Position; }
    
private:
    
    float3 m_Position;
    float3 m_Front;
    float3 m_Up;
    float3 m_Right;
    float3 m_WorldUp;
    
    float m_Yaw;
    float m_Pitch;
    
    float m_MovementSpeed, m_Velocity;
    
    float m_MouseSensitivity;
    float m_Zoom = 90.f;

    CAMERA_MOVEMENT m_CameraMovement;
    
    void UpdateCameraVectors();
    
    float Radians(float degrees) const;
    
    float4x4 LookAt(const float3 eye, const float3 center, const float3 up) const;
    
};

#endif /* Camera_hpp */
