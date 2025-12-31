//
//  Camera.h
//  LightningCore
//
//  Created by Kian Marvi on 12/25/25.
//

#ifndef Camera_h
#define Camera_h

#include <simd/simd.h>

enum class CAMERA_MOVEMENT
{
    FORWARD,
    BACKWARD,
    LEFT,
    RIGHT
};

constexpr float YAW         = -90.0f;
constexpr float PITCH       = 0.0f;
constexpr float SPEED       = 2.5f;
constexpr float SENSITIVITY = 0.1f;
constexpr float ZOOM        = 45.0f;

class Camera
{
public:
    Camera(simd::float3 position = simd::make_float3(0.0f, 0.0f, 0.0), simd::float3 up = simd::make_float3(0.0f, 1.0f, 0.0f), float yaw = YAW, float pitch = PITCH);
    ~Camera();
    
    void ProcessKeyboardInput(const CAMERA_MOVEMENT &direction, float deltaTime);
    void ProcessControllerInput(float deltaTime, float axisValueX, float axisValueY);
    void ProcessMouseMovement(float xOffset, float yOffset, bool contrainPitch = true);
    
    inline simd::float4x4 GetViewMatrix() { return LookAt(m_Position, m_Position + m_Front, m_Up); }
    
private:
    
    simd::float3 m_Position;
    simd::float3 m_Front;
    simd::float3 m_Up;
    simd::float3 m_Right;
    simd::float3 m_WorldUp;
    
    float m_Yaw;
    float m_Pitch;
    
    float m_MovementSpeed, m_Velocity;
    
    float m_MouseSensitivity;
    float m_Zoom;

    CAMERA_MOVEMENT m_CameraMovement;
    
    void UpdateCameraVectors();
    
    float Radians(float degrees);
    
    simd::float4x4 LookAt(const simd::float3 &eye, const simd::float3 &center, const simd::float3 &up);
    
};

#endif /* Camera_hpp */
