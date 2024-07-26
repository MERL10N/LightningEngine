//-------------------------------------------------------------------------------------------------------------------------------------------------------------
//
// Copyright 2024 Apple Inc.
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.
//
//-------------------------------------------------------------------------------------------------------------------------------------------------------------

#include "Controller.h"
#import <GameController/GameController.h>
#import <Foundation/Foundation.h>
#import <CoreMotion/CoreMotion.h>
#include <iostream>

GameController::GameController()
: _haptics(nil)
, _motionManager(nil)
{
    
    [NSNotificationCenter.defaultCenter addObserverForName:GCControllerDidDisconnectNotification
                                                    object:nil
                                                     queue:nil
                                                usingBlock:^(NSNotification * _Nonnull notification) {
        
        
        GCController* controller = (GCController *)(notification.object);
        if (![controller.vendorName isEqualToString:@"Apple Touch Controller"])
        {
            CFRelease(this->_haptics);
            this->_haptics = nil;
        }
    }];
    
   // __block GameController* pOwner = this; // capture as writable by the block
    
    [NSNotificationCenter.defaultCenter addObserverForName:GCControllerDidConnectNotification
                                                    object: nil
                                                     queue: nil
                                                usingBlock:^(NSNotification * _Nonnull notification) {
        
      //  GCController* controller = (GCController *)(notification.object);
        
        // Check if the controller supports haptic feedback and if so prepare to
        // play haptics on it:
        /*
        if (controller.haptics)
        {
            CHHapticEngine* hapticEngine =
            [controller.haptics createEngineWithLocality:GCHapticsLocalityHandles];
            
            if (pOwner->_haptics)
            {
                CFRelease(this->_haptics);
                this->_haptics = nil;
            }
            
        }
         */
    }];
    
    // Accelerometer
    
#if TARGET_OS_IOS
    CMMotionManager* cm = [[CMMotionManager alloc] init];
    if (cm.isAccelerometerAvailable)
    {
        cm.accelerometerUpdateInterval = 1/60.0f;
        [cm startAccelerometerUpdates];
        _motionManager = CFBridgingRetain(cm);
    }
#endif
}

GameController::~GameController()
{
    if (this->_haptics)
        CFRelease(this->_haptics);
    
    if (this->_motionManager)
        CFRelease(this->_motionManager);
}

simd::float3 GameController::accelerometerData() const
{
    if (this->_motionManager)
    {
#if TARGET_OS_IOS
        CMMotionManager* cm = (__bridge CMMotionManager *)this->_motionManager;
        CMAcceleration accel = cm.accelerometerData.acceleration;
        return simd::float3 { (float)accel.x, (float)accel.y, (float)accel.z };
#endif
    }
    return simd::float3{ 0, 0, 0 };
}

bool GameController::isLeftArrowDown() const
{
    return [GCKeyboard.coalescedKeyboard.keyboardInput buttonForKeyCode:GCKeyCodeLeftArrow].pressed;
}

bool GameController::isRightArrowDown() const
{
    return [GCKeyboard.coalescedKeyboard.keyboardInput buttonForKeyCode:GCKeyCodeRightArrow].pressed;
}

bool GameController::isUpArrowDown() const
{
    return [GCKeyboard.coalescedKeyboard.keyboardInput buttonForKeyCode:GCKeyCodeUpArrow].pressed;
}

bool GameController::isDownArrowDown() const
{
    return [GCKeyboard.coalescedKeyboard.keyboardInput buttonForKeyCode:GCKeyCodeDownArrow].pressed;
}

bool GameController::isWKeyDown() const
{
    return [GCKeyboard.coalescedKeyboard.keyboardInput buttonForKeyCode:GCKeyCodeKeyW].pressed;
}

bool GameController::isAKeyDown() const
{
    return [GCKeyboard.coalescedKeyboard.keyboardInput buttonForKeyCode:GCKeyCodeKeyA].pressed;
}

bool GameController::isSKeyDown() const
{
    return [GCKeyboard.coalescedKeyboard.keyboardInput buttonForKeyCode:GCKeyCodeKeyS].pressed;
}

bool GameController::isDKeyDown() const
{
    return [GCKeyboard.coalescedKeyboard.keyboardInput buttonForKeyCode:GCKeyCodeKeyD].pressed;
}


bool GameController::isSpacebarDown() const
{
    return [GCKeyboard.coalescedKeyboard.keyboardInput buttonForKeyCode:GCKeyCodeSpacebar].pressed;
}

float GameController::leftThumbstickX() const
{
    GCExtendedGamepad* gamepad = GCController.controllers.firstObject.extendedGamepad;
    return gamepad.leftThumbstick.xAxis.value;
}

float GameController::rightThumbstickX() const
{
    GCExtendedGamepad* gamepad = GCController.controllers.firstObject.extendedGamepad;
    return gamepad.rightThumbstick.xAxis.value;
}

bool GameController::isButtonADown() const
{
    GCExtendedGamepad* gamepad = GCController.controllers.firstObject.extendedGamepad;
    return gamepad.buttonA.pressed;
}

void GameController::setHapticIntensity(float intensity) const
{

}

void GameController::renderOverlay(MTL::RenderCommandEncoder* pEnc)
{
    // Here you can customize how your touch (virtual) game controller looks
    // by directly issuing Metal commands and render it onscreen.

    // To accomplish this, in IOSViewController.m, customize the Virtual Game
    // Controller to hide its buttons, and render your custom UI from here.

    // Follow the guidance in the HIG to develop a great touch controls
    // experience for your players by surfacing actions relevant to your
    // game's context, and drawing semantically-meaningful glyphs.
}
