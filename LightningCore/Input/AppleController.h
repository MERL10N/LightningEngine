//
//  AppleController.h
//  LightningCore
//
//  Created by Kian Marvi on 12/27/25.
//

#ifndef AppleController_h
#define AppleController_h

class AppleController
{
public:
    AppleController() = default;
    ~AppleController() {}
    
    // Keyboard Input
    bool IsWKeyDown() const;
    bool IsAKeyDown() const;
    bool IsSKeyDown() const;
    bool IsDKeyDown() const;
    
    // Gamepad Input
    float LeftThumbstickX() const;
    float rightThumbstickX() const;
};

#endif /* AppleController_h */
