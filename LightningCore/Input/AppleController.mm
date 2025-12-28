//
//  AppleController.mm
//  LightningCore
//
//  Created by Kian Marvi on 12/27/25.
//

#include "AppleController.h"
#include <GameController/Gamecontroller.h>

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


