//
//  AppleController.mm
//  LightningCore
//
//  Created by Kian Marvi on 12/27/25.
//

#include "AppleController.h"
#include <GameController/Gamecontroller.h>
#include <CoreGraphics/CoreGraphics.h>

bool AppleController::IsWKeyDown() const
{
    return [GCKeyboard.coalescedKeyboard.keyboardInput buttonForKeyCode:GCKeyCodeKeyW].pressed;
}

bool AppleController::IsAKeyDown() const
{
    return [GCKeyboard.coalescedKeyboard.keyboardInput buttonForKeyCode:GCKeyCodeKeyA].pressed;
}

bool AppleController::IsSKeyDown() const
{
    return [GCKeyboard.coalescedKeyboard.keyboardInput buttonForKeyCode:GCKeyCodeKeyS].pressed;
}

bool AppleController::IsDKeyDown() const
{
    return [GCKeyboard.coalescedKeyboard.keyboardInput buttonForKeyCode:GCKeyCodeKeyD].pressed;
}

void AppleController::GetCursorPosition()
{
    CGEventRef event = CGEventCreate(nullptr);
    CGPoint cursor_position = CGEventGetLocation(event);
    CFRelease(event);
}

float AppleController::LeftThumbstickX() const
{
    GCExtendedGamepad* gamepad = GCController.controllers.firstObject.extendedGamepad;
    return (gamepad.leftThumbstick.xAxis.value);
}

float AppleController::LeftThumbstickY() const
{
    GCExtendedGamepad* gamepad = GCController.controllers.firstObject.extendedGamepad;
    return (gamepad.leftThumbstick.yAxis.value);
}

float AppleController::RightThumbstick() const
{
    GCExtendedGamepad* gamepad = GCController.controllers.firstObject.extendedGamepad;
    return (gamepad.rightThumbstick.xAxis.value) || (gamepad.rightThumbstick.yAxis.value);
}

