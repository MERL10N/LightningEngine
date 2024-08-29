#include "Controller.h"
#import <GameController/GameController.h>
#import <Foundation/Foundation.h>
#import <CoreMotion/CoreMotion.h>
#include <ApplicationServices/ApplicationServices.h>
#include <Cocoa/Cocoa.h>
#include <iostream>

Controller::Controller()
{
}

Controller::~Controller()
{
}


bool Controller::isLeftArrowDown() const
{
    return [GCKeyboard.coalescedKeyboard.keyboardInput buttonForKeyCode:GCKeyCodeLeftArrow].pressed;
}

bool Controller::isRightArrowDown() const
{
    return [GCKeyboard.coalescedKeyboard.keyboardInput buttonForKeyCode:GCKeyCodeRightArrow].pressed;
}

bool Controller::isUpArrowDown() const
{
    return [GCKeyboard.coalescedKeyboard.keyboardInput buttonForKeyCode:GCKeyCodeUpArrow].pressed;
}

bool Controller::isDownArrowDown() const
{
    return [GCKeyboard.coalescedKeyboard.keyboardInput buttonForKeyCode:GCKeyCodeDownArrow].pressed;
}

bool Controller::isWKeyDown()
{
    return [GCKeyboard.coalescedKeyboard.keyboardInput buttonForKeyCode:GCKeyCodeKeyW].pressed;
}

bool Controller::isAKeyDown()
{
    return [GCKeyboard.coalescedKeyboard.keyboardInput buttonForKeyCode:GCKeyCodeKeyA].pressed;
}

bool Controller::isSKeyDown()
{
    return [GCKeyboard.coalescedKeyboard.keyboardInput buttonForKeyCode:GCKeyCodeKeyS].pressed;
}

bool Controller::isDKeyDown()
{
    return [GCKeyboard.coalescedKeyboard.keyboardInput buttonForKeyCode:GCKeyCodeKeyD].pressed;
}


bool Controller::isSpacebarDown() const
{
    return [GCKeyboard.coalescedKeyboard.keyboardInput buttonForKeyCode:GCKeyCodeSpacebar].pressed;
}

float Controller::leftThumbstickX()
{
    return GCController.controllers.firstObject.extendedGamepad.leftThumbstick.xAxis.value;
}

float Controller::leftThumbstickY()
{
    return GCController.controllers.firstObject.extendedGamepad.leftThumbstick.yAxis.value;
}

float Controller::rightThumbstickX()
{
    return GCController.controllers.firstObject.extendedGamepad.rightThumbstick.xAxis.value;
}

float Controller::rightThumbstickY()
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

CGPoint Controller::getMousePosition() 
{
    CGEventRef event = CGEventCreate(nullptr);
    CGPoint location = CGEventGetLocation(event);
    CFRelease(event);
    return location;
}

void Controller::hideCursor() 
{
    [NSCursor hide];
    CGAssociateMouseAndMouseCursorPosition(true);
}

void Controller::showCursor() 
{ 
    [NSCursor unhide];
    CGAssociateMouseAndMouseCursorPosition(true);
}

bool Controller::isLeftMouseClicked()
{
    return ([NSEvent pressedMouseButtons] & ((1 << 0))) != 0;
}
bool Controller::isRightMouseClicked()
{
    return ([NSEvent pressedMouseButtons] & ((1 << 1))) != 0;
}



