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

Mesh MeshBuilder::GenerateQuad(MTL::Device *device, const char* textureFile)
{
    Vertex vertices[] =
    {
        {{-0.75, -0.75, 0.0}, {1.0, 0.0, 0.0}, {0.0, 0.0}},
        {{ 0.75, -0.75, 0.0}, {0.0, 1.0, 0.0}, {1.0, 0.0}},
        {{ 0.75,  0.75, 0.0}, {0.0, 0.0, 1.0}, {1.0, 1.0}},
        {{-0.75,  0.75, 0.0}, {0.0, 1.0, 0.0}, {0.0, 1.0}},
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

MeshBuilder::~MeshBuilder()
{
    if (m_Mesh.texture)
    {
        delete m_Mesh.texture;
        m_Mesh.texture = nullptr;
    }
}


