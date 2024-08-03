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

// For CFIndex, CFTypeRef
#include <CoreFoundation/CFBase.h>
#include <unordered_map>

#include <simd/simd.h>

namespace MTL
{
    class RenderCommandEncoder;
}

class Controller
{
public:
    // Default functions already implemented by Apple
    Controller();
    ~Controller();
    
    bool isLeftArrowDown() const;
    bool isRightArrowDown() const;
    bool isUpArrowDown() const;
    bool isDownArrowDown() const;
    
    void setHapticIntensity(float intensity) const;
    
    void  renderOverlay(MTL::RenderCommandEncoder* pEnc);
    bool isButtonADown() const;
    bool isSpacebarDown() const;
    simd::float3 accelerometerData() const;

    // Below are functions I have added to the class
    bool isWKeyDown() const;
    bool isAKeyDown() const;
    bool isSKeyDown() const;
    bool isDKeyDown() const;

    float leftThumbstickX() const;
    float leftThumbstickY() const;
    float rightThumbstickX() const;
    float rightThumbstickY() const;
   
private:
    CFTypeRef                         _haptics;
    CFTypeRef                         _motionManager;
};

#endif // CONTROLLER_H
