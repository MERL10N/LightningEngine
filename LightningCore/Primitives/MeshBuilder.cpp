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

void MeshBuilder::GenerateTriangle(void* p_VertexBuffer)
{
    constexpr float vertices[] =
    {
        // positions         // colors
         0.5f, -0.5f, 0.0f,  1.0f, 0.0f, 0.0f,   // bottom right
        -0.5f, -0.5f, 0.0f,  0.0f, 1.0f, 0.0f,   // bottom left
         0.0f,  0.5f, 0.0f,  0.0f, 0.0f, 1.0f    // top
    };
    
#ifdef __APPLE__
    return static_cast<MetalVertexBuffer*>(p_VertexBuffer)->BindBuffer(vertices, sizeof(vertices));
#endif
}

void MeshBuilder::GenerateQuad(void* p_VertexBuffer, const float p_Width, const float p_Height)
{
    const float halfWidth = p_Width * 0.5f;
    const float halfHeight = p_Height * 0.5f;
    
    const float vertices[] =
    {
           // positions                    // colors           // texture coords
          -halfWidth,  halfHeight, 0.0f,   1.0f, 1.0f, 0.0f,   0.0f, 1.0f,   // top left
          -halfWidth, -halfHeight, 0.0f,   0.0f, 0.0f, 1.0f,   0.0f, 0.0f,   // bottom left
           halfWidth,  halfHeight, 0.0f,   1.0f, 0.0f, 0.0f,   1.0f, 1.0f,   // top right
           halfWidth, -halfHeight, 0.0f,   0.0f, 1.0f, 0.0f,   1.0f, 0.0f    // bottom right
    };
    
#ifdef __APPLE__
    return static_cast<MetalVertexBuffer*>(p_VertexBuffer)->BindBuffer(vertices, 4 * 8 * sizeof(vertices));
#endif
}
