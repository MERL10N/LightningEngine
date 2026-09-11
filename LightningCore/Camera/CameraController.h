//  CameraController.h
//  LightningCore
//
//  Created by Kian Marvi on 8/28/26.
//

#ifndef CameraController_hpp
#define CameraController_hpp
#include <hlsl++.h>

#include "Input/AppleController.h"

class Camera;

class CameraController
{
public:
    CameraController() = default;
    CameraController(const float aspectRatio, const bool rotation = false);
    
    void Update(Camera& camera, const float dt);
    
private:
    bool        m_Rotation;
    float       m_AspectRatio;
    float       m_CameraRotation    = 0.0f;
    float       m_CameraTranslationSpeed = 1.0f;
    
    AppleController m_AppleController;
    
};

#endif /* CameraController_hpp */
