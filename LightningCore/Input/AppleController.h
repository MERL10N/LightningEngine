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
    
    // Mouse Cursor Position
    void GetCursorPosition();
    
    // Gamepad Input
    float LeftThumbstickX() const;
    float LeftThumbstickY() const;
    float RightThumbstick() const;
};

#endif /* AppleController_h */
