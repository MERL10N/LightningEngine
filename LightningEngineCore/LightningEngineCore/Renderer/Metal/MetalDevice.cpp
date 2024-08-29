//
//  MetalDevice.cpp
//  LightningEngineCore
//
//  Created by Kian Marvi on 8/27/24.
//

#include "MetalDevice.h"
#include <Metal/Metal.hpp>

void MetalDevice::Init()
{
    device = MTL::CreateSystemDefaultDevice();
}

MTL::Device* MetalDevice::GetDevice()
{
    return device;
}

void MetalDevice::Cleanup()
{
    device->release();
    device = nullptr;
}

