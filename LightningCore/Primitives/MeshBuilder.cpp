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

void MeshBuilder::GenerateTriangle(void* p_Device, void* p_VertexBuffer, void* p_RenderEncoder)
{
    constexpr float vertices[] =
    {
        // positions         // colors
         0.5f, -0.5f, 0.0f,  1.0f, 0.0f, 0.0f,   // bottom right
        -0.5f, -0.5f, 0.0f,  0.0f, 1.0f, 0.0f,   // bottom left
         0.0f,  0.5f, 0.0f,  0.0f, 0.0f, 1.0f    // top
    };
    
#ifdef __APPLE__
    return static_cast<MetalVertexBuffer*>(p_VertexBuffer)->BindBuffer(vertices, sizeof(vertices), static_cast<MTL::RenderCommandEncoder*>(p_RenderEncoder));
#endif
}
