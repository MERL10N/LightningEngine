//
//  MetalDevice.hpp
//  LightningEngineCore
//
//  Created by Kian Marvi on 8/27/24.
//

#ifndef MetalDevice_hpp
#define MetalDevice_hpp

namespace MTL
{
    class Device;
}

class MetalDevice
{
public:
    
    static void Init();
    static void Cleanup();
    
    static MTL::Device* GetDevice();
private:
    static MTL::Device* device;
};

#endif /* MetalDevice_hpp */
