/*
    MeshBuilder.cpp
    LightningEngine

    Created by Kian Marvi on 2/2/24.
*/

#include "MeshBuilder.h"
#include "VertexData.h"

#include <System/ImageLoader.h>

/**
 @brief Generate a quad and load it into Metal
 @param metalDevice A metal compatible device (Either mac or iOS)
 */

MTL::Buffer* CMeshBuilder::GenerateQuad(MTL::Device* metalDevice)
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
