//
//  MeshBuilder.cpp
//  LightningCore
//
//  Created by Kian Marvi on 5/6/25.
//

#include "MeshBuilder.h"

#ifdef __APPLE__
    #include "Metal/Metal.hpp"
#include "Renderer/Metal/MetalBuffer.h"
#include "Renderer/Metal/MetalTexture.h"
#endif


MeshBuilder::~MeshBuilder()
{
    if (m_Mesh.texture)
    {
        delete m_Mesh.texture;
        m_Mesh.texture = nullptr;
    }
}

Mesh MeshBuilder::GenerateQuad(MTL::Device *device, const char* textureFile)
{
    Vertex vertices[] =
    {
        {{-0.5f, -0.5f, 0.0f}, {1.0, 1.0, 1.0}, {0.0, 0.0}},
        {{ 0.5f, -0.5f, 0.0f}, {1.0, 1.0, 1.0}, {1.0, 0.0}},
        {{ 0.5f,  0.5f, 0.0f}, {1.0, 1.0, 1.0}, {1.0, 1.0}},
        {{-0.5f,  0.5f, 0.0f}, {1.0, 1.0, 1.0}, {0.0, 1.0}},
    };
    
    NS::UInteger vertexBufferSize = 4 * sizeof(Vertex);
        
    ushort indices[4] = {0, 1, 3, 2};
    NS::UInteger indexBufferSize = 4 * sizeof(ushort);
        
    //vertex buffer
    m_Mesh.vertexBuffer = device->newBuffer(vertexBufferSize, MTL::ResourceStorageModeShared);
    memcpy(m_Mesh.vertexBuffer->contents(), vertices, vertexBufferSize);
        
    //index buffer
    m_Mesh.indexBuffer = device->newBuffer(indexBufferSize, MTL::ResourceStorageModeShared);
    memcpy(m_Mesh.indexBuffer->contents(), indices, indexBufferSize);
    
    m_Mesh.texture = new MetalTexture(textureFile);
    m_Mesh.texture->SetMetalDevice(device);
    
    return m_Mesh;
}

Mesh MeshBuilder::GenerateCube(MTL::Device *device, const char *texture)
{
   
    Vertex vertices[] =
    {
        // Front face
        {{-0.5f, -0.5f, 0.5f}, {1.0, 1.0, 1.0}, {0.0, 0.0}}, // 0
        {{ 0.5f, -0.5f, 0.5f}, {1.0, 1.0, 1.0}, {1.0, 0.0}}, // 1
        {{ 0.5f,  0.5f, 0.5f}, {1.0, 1.0, 1.0}, {1.0, 1.0}}, // 2
        {{-0.5f,  0.5f, 0.5f}, {1.0, 1.0, 1.0}, {0.0, 1.0}}, // 3
        
        // Back face
        {{-0.5f, -0.5f, -0.5f}, {1.0, 1.0, 1.0}, {0.0, 0.0}}, // 4
        {{ 0.5f, -0.5f, -0.5f}, {1.0, 1.0, 1.0}, {1.0, 0.0}}, // 5
        {{ 0.5f,  0.5f, -0.5f}, {1.0, 1.0, 1.0}, {1.0, 1.0}}, // 6
        {{-0.5f,  0.5f, -0.5f}, {1.0, 1.0, 1.0}, {0.0, 1.0}}, // 7
        
        // Left face
        {{-0.5f,  0.5f,  0.5f}, {1.0, 1.0, 1.0}, {0.0, 0.0}}, // 8
        {{-0.5f,  0.5f, -0.5f}, {1.0, 1.0, 1.0}, {1.0, 0.0}}, // 9
        {{-0.5f, -0.5f, -0.5f}, {1.0, 1.0, 1.0}, {1.0, 1.0}}, // 10
        {{-0.5f, -0.5f,  0.5f}, {1.0, 1.0, 1.0}, {0.0, 1.0}}, // 11
        
        // Right face
        {{0.5f,  0.5f,  0.5f}, {1.0, 1.0, 1.0}, {0.0, 0.0}}, // 12
        {{0.5f,  0.5f, -0.5f}, {1.0, 1.0, 1.0}, {1.0, 0.0}}, // 13
        {{0.5f, -0.5f, -0.5f}, {1.0, 1.0, 1.0}, {1.0, 1.0}}, // 14
        {{0.5f, -0.5f,  0.5f}, {1.0, 1.0, 1.0}, {0.0, 1.0}}, // 15
        
        // Top face
        {{-0.5f,  0.5f, -0.5f}, {1.0, 1.0, 1.0}, {0.0, 0.0}}, // 16
        {{ 0.5f,  0.5f, -0.5f}, {1.0, 1.0, 1.0}, {1.0, 0.0}}, // 17
        {{ 0.5f,  0.5f, -0.5f}, {1.0, 1.0, 1.0}, {1.0, 1.0}}, // 18
        {{-0.5f,  0.5f, -0.5f}, {1.0, 1.0, 1.0}, {0.0, 1.0}}, // 19
        
        // Bottom face
        {{-0.5f, -0.5f, -0.5f}, {1.0, 1.0, 1.0}, {0.0, 0.0}}, // 20
        {{ 0.5f, -0.5f, -0.5f}, {1.0, 1.0, 1.0}, {1.0, 0.0}}, // 21
        {{ 0.5f, -0.5f, -0.5f}, {1.0, 1.0, 1.0}, {1.0, 1.0}}, // 22
        {{-0.5f, -0.5f, -0.5f}, {1.0, 1.0, 1.0}, {0.0, 1.0}}, // 23
       
    };
    
    uint16_t indices[] =
    {
        0, 1, 2, 2, 3, 0,
        4, 5, 6, 6, 7, 4,
        8, 9, 10, 10, 11, 8,
        16, 17, 18, 18, 19, 16,
        20, 21, 22, 22, 23, 20
        
    };
    
    // vertex buffer
    m_Mesh.vertexBuffer = device->newBuffer(4 * sizeof(Vertex), MTL::ResourceStorageModeShared);
    memcpy(m_Mesh.vertexBuffer->contents(), vertices, sizeof(Vertex));
    
    // Index buffer
    m_Mesh.indexBuffer = device->newBuffer(4 * sizeof(uint16_t), MTL::ResourceStorageModeShared);
    memcpy(m_Mesh.indexBuffer->contents(), indices, sizeof(uint16_t));
    
    m_Mesh.texture = new MetalTexture(texture);
    m_Mesh.texture->SetMetalDevice(device);
    
    // Transformation buffer
    //m_Mesh.transformationBuffer = device->newBuffer(sizeof(Transform), MTL::ResourceStorageModeShared);
    return m_Mesh;
     
}



