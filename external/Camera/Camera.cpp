//
//  Camera.cpp
//  LightningEngine
//
//  Created by Kian Marvi on 7/24/24.
//

#include "Camera.h"

Camera::Camera(float3 position, float3 up, float Yaw, float Pitch)
: Front(make_float3(0.f, 0.f, -1.f))
, MovementSpeed(SPEED)
, MouseSensitivity(SENSITIVITY)
, Zoom(ZOOM)
, Position(position)
, Yaw(Yaw)
,Pitch(Pitch)
{
    UpdateCameraVectors();
}

Camera::Camera(float &posX, float &posY, float &posZ, float &upX, float &upY, float &upZ, float &yaw, float &pitch)
: Front(make_float3(0.f, 0.f, -1.f))
, MovementSpeed(SPEED)
, MouseSensitivity(SENSITIVITY)
, Zoom(ZOOM)
{
    Position = make_float3(posX, posY, posZ);
    WorldUp = make_float3(upX, upY, upZ);
    Yaw = yaw;
    Pitch = pitch;
    UpdateCameraVectors();
}

float4x4 Camera::GetViewMatrix()
{
    return LookAt(Position, Position + Front, Up);
}

float4x4 Camera::LookAt(const float3 &eye, const float3 &center, const float3 &up)
{
    float3 z = normalize(eye - center);
    float3 x = normalize(cross(up, z));
    float3 y = cross(z, x);

    float4x4 viewMatrix;
    viewMatrix.columns[0] = make_float4(x, 0);
    viewMatrix.columns[1] = make_float4(y, 0);
    viewMatrix.columns[2] = make_float4(z, 0);
    viewMatrix.columns[3] = make_float4(eye, 1);

    return inverse(viewMatrix);
}

inline float Camera::Radians(float &degrees)
{
    return degrees * M_PI / 180.0f;
}

void Camera::UpdateCameraVectors()
{
    // calculate the new Front vector
    float3 front;
    front.x = cos(Radians(Yaw)) * cos(Radians(Pitch));
    front.y = sin(Radians(Pitch));
    front.z = sin(Radians(Yaw)) * cos(Radians(Pitch));
    Front = normalize(front);
    
    // also re-calculate the Right and Up vector
    Right = normalize(cross(Front, WorldUp));  // normalize the vectors, because their length gets closer to 0 the more you look up or down which results in slower movement.
    Up    = normalize(cross(Right, Front));
}
