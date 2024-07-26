/*
      Camera.h
      LightningEngine
      Created by Kian Marvi on 7/24/24.
*/

#ifndef CAMERA_H
#define CAMERA_H

#include <simd/simd.h>

#include <vector>

using namespace simd;

// Enum values for the several possible camera movements
enum CAMERA_MOVEMENT
{
    FORWARD,
    BACKWARD,
    LEFT,
    RIGHT
};

// Assign default camera values
const float YAW         = -90.0f;
const float PITCH       =  0.0f;
const float SPEED       =  2.5f;
const float SENSITIVITY =  0.1f;
const float ZOOM        =  45.0f;

class Camera 
{
public:
    
    // Constructor with vectors
    Camera(float3 position = make_float3(0.f, 0.f, 0.f), float3 Up = make_float3(0.f, 1.f, 0.f), float Yaw = YAW, float Pitch = PITCH);
    
    // Constructor with scalar values
    Camera(float &posX, float &posY, float &posZ, float &upX, float &upY, float &upZ, float &yaw, float &pitch);
    
    float4x4 GetViewMatrix();
    
    // Process input from any keyboard-like input system
    void ProcessKeyboard(const CAMERA_MOVEMENT &direction, float &deltaTime);
    
    // Processes input received from a mouse input system
    void ProcessMouseMovement(float &xOffset, float &yOffset, bool constraintPitch = true);
    
    float GetZoom();
    
    float3 GetCameraLocation();

private:
    
    float4x4 LookAt(const float3 &eye, const float3 &center, const float3 &up);
    
    // Calculates the front vector from the Camera's (updated) Euler Angles
    void UpdateCameraVectors();
    
    inline float Radians(float &degrees);
    
    // Camera attributes
    float3 Position;
    float3 Front;
    float3 Up;
    float3 Right;
    float3 WorldUp;
    
    // Euler angles
    float Yaw;
    float Pitch;
    
    // Camera options
    float MovementSpeed;
    float MouseSensitivity;
    float Zoom;
};


#endif /* Camera_h */
