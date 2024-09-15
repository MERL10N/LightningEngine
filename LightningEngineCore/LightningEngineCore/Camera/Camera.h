/**
     File: Camera.h
     @brief
     This camera class has been adapted from LearnOpenGL to be used with Apple's SIMD library instead of GLM.
     Minor enhancements have been made for better clarity and efficiency (e.g using initializer list and pass by reference).
      
     Extensions have also been made to support input from a Gamepad.
     @copyright
        
     Original code by: Joey De Vries
     Link:  https://learnopengl.com/Getting-started/Camera
 
     Extended by: Kian Heydari Marvi
     Date: July 2024
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


class Camera 
{
public:
    
    // Default Constructor
    Camera();
    
    // Constructor with scalar values
    Camera(float &posX, float &posY, float &posZ, float &upX, float &upY, float &upZ, float &yaw, float &pitch);
    
    float4x4 GetViewMatrix();
    
    // Process input from any keyboard-like input system
    void ProcessKeyboard(const CAMERA_MOVEMENT &direction, float &deltaTime);
    
    // Processes input received from a mouse input system
    void ProcessMouseMovement(float &xOffset, float &yOffset, const bool &constraintPitch);
    
    // Process input from any gamepad input system
    void ProcessGamepadLeftJoystick(const float &deltaTime, const float &axisXValue, const float &axisYValue);
    
    void ProcessGamepadRightJoystick(const float &axisXValue, const float &axisYValue, const bool &constrainTBool);
    
    float GetZoom();
    
    inline float3& GetCameraPosition() { return Position; }

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
    float JoystickRightAxis;
    float Zoom;
    
    // Speed variables
    float velocity;
    float deltaTime;
};


#endif /* Camera_h */
