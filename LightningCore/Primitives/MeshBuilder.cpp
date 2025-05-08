//
//  MeshBuilder.cpp
//  LightningCore
//
//  Created by Kian Marvi on 5/6/25.
//

#include "MeshBuilder.h"

#ifdef __APPLE__
    #include "../Renderer/Metal/MetalBuffer.h"
#endif

VertexBuffer* MeshBuilder::GenerateTriangle(void* p_Device)
{
    constexpr float vertices[] =
    {
        // positions         // colors
         0.5f, -0.5f, 0.0f,  1.0f, 0.0f, 0.0f,   // bottom right
        -0.5f, -0.5f, 0.0f,  0.0f, 1.0f, 0.0f,   // bottom left
         0.0f,  0.5f, 0.0f,  0.0f, 0.0f, 1.0f    // top
    };
    
#ifdef __APPLE__
    return new MetalVertexBuffer(vertices, sizeof(vertices), static_cast<MTL::Device*>(p_Device));
#endif
}
