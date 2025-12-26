//
//  Camera.h
//  LightningCore
//
//  Created by Kian Marvi on 12/25/25.
//

#ifndef Camera_h
#define Camera_h

#include <simd/simd.h>

class Camera
{
public:
    Camera() = default;
    Camera(float, float, simd::float3, simd::float3);
    ~Camera();
private:
    simd::float3 m_Position;
    
};

#endif /* Camera_hpp */
