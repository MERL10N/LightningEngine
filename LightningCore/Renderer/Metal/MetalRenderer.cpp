//
// Created by Kian Marvi on 11/24/24.
//

#include "MetalRenderer.h"
#include <Metal/Metal.hpp>
#include <MetalKit/MetalKit.hpp>

MetalRenderer::MetalRenderer(MTL::Device *metalDevice)
    : metalDevice(metalDevice),
      metalCommandQueue(metalDevice->newCommandQueue()),
      metalCommandBuffer(nullptr),
      renderPassDescriptor(nullptr),
      renderCommandEncoder(nullptr)
{
}

void MetalRenderer::InitMetal(MTL::Device* metalDevice)
{
    this->metalDevice = metalDevice;
    metalCommandQueue = metalDevice->newCommandQueue();
    metalCommandBuffer = nullptr;
    renderPassDescriptor = nullptr;
    renderCommandEncoder = nullptr;
}


void MetalRenderer::CleanMetal()
{
    if (metalDevice)
    {
        metalDevice->release();
        metalDevice = nullptr;
    }

    if (metalCommandQueue)
    {
        metalCommandQueue->release();
        metalCommandQueue = nullptr;
    }
}

void MetalRenderer::RenderMetal(const MTK::View *metalKitView)
{
    metalCommandBuffer = metalCommandQueue->commandBuffer();
    renderPassDescriptor = metalKitView->currentRenderPassDescriptor();
    renderCommandEncoder = metalCommandBuffer->renderCommandEncoder(renderPassDescriptor);
    renderCommandEncoder->endEncoding();
    metalCommandBuffer->presentDrawable(metalKitView->currentDrawable());
    metalCommandBuffer->commit();
}


