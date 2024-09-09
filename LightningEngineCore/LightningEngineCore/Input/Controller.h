/**
     File: Controller.h
     @brief
        This class abstracts the Objective-C code from Apple's Game Controller framework.
        Originally authored by Apple but has been extended further for additional functionality.
     @copyright
        Copyright [2024] [Apple Inc]

        Licensed under the Apache License, Version 2.0 (the "License");
        you may not use this file except in compliance with the License.
        You may obtain a copy of the License at

        http://www.apache.org/licenses/LICENSE-2.0

        Unless required by applicable law or agreed to in writing, software
        distributed under the License is distributed on an "AS IS" BASIS,
        WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
        See the License for the specific language governing permissions and
        limitations under the License.
 
     Extended by: Kian Heydari Marvi
     Date: July 2024
*/

#ifndef CONTROLLER_H
#define CONTROLLER_H

#include <unordered_map>


#include <simd/simd.h>

namespace MTL
{
    class RenderCommandEncoder;
}

struct CGPoint;

class Controller
{
public:
    // Default functions already implemented by Apple
    Controller();
    ~Controller();
    
    bool IsLeftArrowDown() const;
    bool IsRightArrowDown() const;
    bool IsUpArrowDown() const;
    bool IsDownArrowDown() const;
    
    void setHapticIntensity(float intensity) const;
    
    void  renderOverlay(MTL::RenderCommandEncoder* pEnc);
    bool isButtonADown() const;
    bool IsSpacebarDown() const;
    simd::float3 accelerometerData() const;

    // Below are functions I have added to the class
    
    //Keyboard Input functions
    static bool IsWKeyDown();
    static bool IsAKeyDown();
    static bool IsSKeyDown();
    static bool IsDKeyDown();

    // Gamepad input functions
    static float LeftThumbstickX();
    static float LeftThumbstickY();
    static float RightThumbstickX();
    static float RightThumbstickY();
    
    // Mouse input functions
    static CGPoint GetMousePosition();
    static void HideCursor();
    static void ShowCursor();
    static bool isLeftMouseClicked();
    static bool IsRightMouseClicked();

};

#endif // CONTROLLER_H
