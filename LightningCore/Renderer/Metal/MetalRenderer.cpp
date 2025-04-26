//
// Created by Kian Marvi on 11/24/24.
//

#include "MetalRenderer.h"
#include "Metal/Metal.hpp"
#include "MetalKit/MetalKit.hpp"

MetalRenderer::MetalRenderer(MTL::Device* metalDevice)
: metalDevice(metalDevice),
  metalCommandQueue(metalDevice->newCommandQueue()),
  metalCommandBuffer(nullptr),
  renderPassDescriptor(nullptr),
  renderCommandEncoder(nullptr),
  shader("Shaders/Triangle.metal", metalDevice),
  timer(Timer())
{
    CreateTriangle();
}

MetalRenderer::~MetalRenderer()
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
    
    if (triangleVertexBuffer)
    {
        triangleVertexBuffer->release();
        triangleVertexBuffer = nullptr;
    }
}

void MetalRenderer::CreateTriangle()
{
    triangleVertexBuffer = MeshBuilder::GenerateTriangle(metalDevice);
}

void MetalRenderer::Render(const MTK::View *metalKitView)
{
    metalCommandBuffer = metalCommandQueue->commandBuffer();
    renderPassDescriptor = metalKitView->currentRenderPassDescriptor();
    renderCommandEncoder = metalCommandBuffer->renderCommandEncoder(renderPassDescriptor);
    
    renderCommandEncoder->setRenderPipelineState(shader.GetRenderPSO());
    renderCommandEncoder->setVertexBuffer(triangleVertexBuffer, 0, 0);
    renderCommandEncoder->drawPrimitives(MTL::PrimitiveType::PrimitiveTypeTriangle, NS::UInteger{0}, NS::UInteger{3});
    
    renderCommandEncoder->endEncoding();
    metalCommandBuffer->presentDrawable( metalKitView->currentDrawable());
    metalCommandBuffer->commit();
}
