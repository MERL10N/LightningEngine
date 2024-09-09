#include "Controller.h"
#import <GameController/GameController.h>
#import <Foundation/Foundation.h>
#import <CoreMotion/CoreMotion.h>
#include <ApplicationServices/ApplicationServices.h>
#include <Cocoa/Cocoa.h>
#include <iostream>


bool Controller::IsLeftArrowDown() const
{
    return [GCKeyboard.coalescedKeyboard.keyboardInput buttonForKeyCode:GCKeyCodeLeftArrow].pressed;
}

bool Controller::IsRightArrowDown() const
{
    return [GCKeyboard.coalescedKeyboard.keyboardInput buttonForKeyCode:GCKeyCodeRightArrow].pressed;
}

bool Controller::IsUpArrowDown() const
{
    return [GCKeyboard.coalescedKeyboard.keyboardInput buttonForKeyCode:GCKeyCodeUpArrow].pressed;
}

bool Controller::IsDownArrowDown() const
{
    return [GCKeyboard.coalescedKeyboard.keyboardInput buttonForKeyCode:GCKeyCodeDownArrow].pressed;
}

bool Controller::IsWKeyDown()
{
    return [GCKeyboard.coalescedKeyboard.keyboardInput buttonForKeyCode:GCKeyCodeKeyW].pressed;
}

bool Controller::IsAKeyDown()
{
    return [GCKeyboard.coalescedKeyboard.keyboardInput buttonForKeyCode:GCKeyCodeKeyA].pressed;
}

bool Controller::IsSKeyDown()
{
    return [GCKeyboard.coalescedKeyboard.keyboardInput buttonForKeyCode:GCKeyCodeKeyS].pressed;
}

bool Controller::IsDKeyDown()
{
    return [GCKeyboard.coalescedKeyboard.keyboardInput buttonForKeyCode:GCKeyCodeKeyD].pressed;
}

bool Controller::IsSpacebarDown() const
{
    return [GCKeyboard.coalescedKeyboard.keyboardInput buttonForKeyCode:GCKeyCodeSpacebar].pressed;
}

float Controller::LeftThumbstickX()
{
    return GCController.controllers.firstObject.extendedGamepad.leftThumbstick.xAxis.value;
}

float Controller::LeftThumbstickY()
{
    return GCController.controllers.firstObject.extendedGamepad.leftThumbstick.yAxis.value;
}

float Controller::RightThumbstickX()
{
    return GCController.controllers.firstObject.extendedGamepad.rightThumbstick.xAxis.value;
}

float Controller::RightThumbstickY()
{
    return GCController.controllers.firstObject.extendedGamepad.rightThumbstick.yAxis.value;
}


void Controller::setHapticIntensity(float intensity) const
{
}

void Controller::renderOverlay(MTL::RenderCommandEncoder* pEnc)
{
    // Here you can customize how your touch (virtual) game controller looks
    // by directly issuing Metal commands and render it onscreen.

    // To accomplish this, in IOSViewController.m, customize the Virtual Game
    // Controller to hide its buttons, and render your custom UI from here.

    // Follow the guidance in the HIG to develop a great touch controls
    // experience for your players by surfacing actions relevant to your
    // game's context, and drawing semantically-meaningful glyphs.
}

CGPoint Controller::GetMousePosition() 
{
    CGEventRef event = CGEventCreate(nullptr);
    CGPoint location = CGEventGetLocation(event);
    CFRelease(event);
    return location;
}

void Controller::HideCursor() 
{
    [NSCursor hide];
    CGAssociateMouseAndMouseCursorPosition(true);
}

void Controller::ShowCursor() 
{ 
    [NSCursor unhide];
    CGAssociateMouseAndMouseCursorPosition(true);
}

bool Controller::isLeftMouseClicked()
{
    return ([NSEvent pressedMouseButtons] & ((1 << 0))) != 0;
}
bool Controller::IsRightMouseClicked()
{
    return ([NSEvent pressedMouseButtons] & ((1 << 1))) != 0;
}



