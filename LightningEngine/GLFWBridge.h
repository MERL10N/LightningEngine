//
//  GLFWBridge.hpp
//  LightningEngine
//
//  Created by Kian Marvi on 4/2/24.

#ifndef GLFWBridge_h
#define GLFWBridge_h

struct GLFWwindow;

namespace CA
{
    class MetalLayer;
}
namespace GLFWBridge
{
    void AddLayerToWindow(GLFWwindow* window, CA::MetalLayer* layer);
}
#endif /* GLFWBridge_hpp */
