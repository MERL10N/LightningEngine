/*
    MeshBuilder.cpp
    LightningEngine

    Created by Kian Marvi on 2/2/24.
*/

#include "MeshBuilder.h"
#include "VertexData.h"
#include <Metal/Metal.hpp>
#include <Renderer/Metal/MetalDevice.h>

/**
 @brief Generate a quad and load it into Metal
 @param metalDevice A metal compatible device (Either mac or iOS)
 */

MTL::Buffer* MeshBuilder::GenerateQuad(MTL::Device* metalDevice)
{
    VertexData squareVertices[]
    {
            {{-0.5, -0.5,  0.5, 1.0f}, {0.0f, 0.0f}},
            {{-0.5,  0.5,  0.5, 1.0f}, {0.0f, 1.0f}},
            {{ 0.5,  0.5,  0.5, 1.0f}, {1.0f, 1.0f}},
            {{-0.5, -0.5,  0.5, 1.0f}, {0.0f, 0.0f}},
            {{ 0.5,  0.5,  0.5, 1.0f}, {1.0f, 1.0f}},
            {{ 0.5, -0.5,  0.5, 1.0f}, {1.0f, 0.0f}}
    };
    
    MTL::Buffer* quadVertexBuffer = metalDevice->newBuffer(&squareVertices, sizeof(squareVertices), MTL::ResourceStorageModeShared);
    return quadVertexBuffer;
}

MTL::Buffer* MeshBuilder::GenerateCube()
{
    VertexData cubeVertices[] = {
            // Front face
            {{-0.5, -0.5, 0.5, 1.0}, {0.0, 0.0}},
            {{0.5, -0.5, 0.5, 1.0}, {1.0, 0.0}},
            {{0.5, 0.5, 0.5, 1.0}, {1.0, 1.0}},
            {{0.5, 0.5, 0.5, 1.0}, {1.0, 1.0}},
            {{-0.5, 0.5, 0.5, 1.0}, {0.0, 1.0}},
            {{-0.5, -0.5, 0.5, 1.0}, {0.0, 0.0}},

            // Back face
            {{0.5, -0.5, -0.5, 1.0}, {0.0, 0.0}},
            {{-0.5, -0.5, -0.5, 1.0}, {1.0, 0.0}},
            {{-0.5, 0.5, -0.5, 1.0}, {1.0, 1.0}},
            {{-0.5, 0.5, -0.5, 1.0}, {1.0, 1.0}},
            {{0.5, 0.5, -0.5, 1.0}, {0.0, 1.0}},
            {{0.5, -0.5, -0.5, 1.0}, {0.0, 0.0}},

            // Top face
            {{-0.5, 0.5, 0.5, 1.0}, {0.0, 0.0}},
            {{0.5, 0.5, 0.5, 1.0}, {1.0, 0.0}},
            {{0.5, 0.5, -0.5, 1.0}, {1.0, 1.0}},
            {{0.5, 0.5, -0.5, 1.0}, {1.0, 1.0}},
            {{-0.5, 0.5, -0.5, 1.0}, {0.0, 1.0}},
            {{-0.5, 0.5, 0.5, 1.0}, {0.0, 0.0}},

            // Bottom face
            {{-0.5, -0.5, -0.5, 1.0}, {0.0, 0.0}},
            {{0.5, -0.5, -0.5, 1.0}, {1.0, 0.0}},
            {{0.5, -0.5, 0.5, 1.0}, {1.0, 1.0}},
            {{0.5, -0.5, 0.5, 1.0}, {1.0, 1.0}},
            {{-0.5, -0.5, 0.5, 1.0}, {0.0, 1.0}},
            {{-0.5, -0.5, -0.5, 1.0}, {0.0, 0.0}},

            // Left face
            {{-0.5, -0.5, -0.5, 1.0}, {0.0, 0.0}},
            {{-0.5, -0.5, 0.5, 1.0}, {1.0, 0.0}},
            {{-0.5, 0.5, 0.5, 1.0}, {1.0, 1.0}},
            {{-0.5, 0.5, 0.5, 1.0}, {1.0, 1.0}},
            {{-0.5, 0.5, -0.5, 1.0}, {0.0, 1.0}},
            {{-0.5, -0.5, -0.5, 1.0}, {0.0, 0.0}},

            // Right face
            {{0.5, -0.5, 0.5, 1.0}, {0.0, 0.0}},
            {{0.5, -0.5, -0.5, 1.0}, {1.0, 0.0}},
            {{0.5, 0.5, -0.5, 1.0}, {1.0, 1.0}},
            {{0.5, 0.5, -0.5, 1.0}, {1.0, 1.0}},
            {{0.5, 0.5, 0.5, 1.0}, {0.0, 1.0}},
            {{0.5, -0.5, 0.5, 1.0}, {0.0, 0.0}},
        };
    
    MTL::Buffer* cubeVertexBuffer = MetalDevice::GetDevice()->newBuffer(&cubeVertices,
                                                                        sizeof(cubeVertices),
                                                                        MTL::ResourceStorageModeShared);
    return cubeVertexBuffer;
}
