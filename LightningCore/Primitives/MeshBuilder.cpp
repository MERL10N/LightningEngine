//
//  MeshBuilder.cpp
//  LightningCore
//
//  Created by Kian Marvi on 5/6/25.
//

#include "MeshBuilder.h"

#include <Metal/Metal.hpp>
#include "Renderer/Metal/MetalTexture.h"
#include "Renderer/Metal/MetalBuffer.h"

Mesh_2D MeshBuilder::GenerateQuadWithTexture(MTL::Device *device, const char* textureFile)
{
    constexpr Vertex vertices[] =
    {
        {{-0.5f, -0.5f, 0.0f}, {1.0, 1.0, 1.0}, {0.0, 0.0}},
        {{ 0.5f, -0.5f, 0.0f}, {1.0, 1.0, 1.0}, {1.0, 0.0}},
        {{ 0.5f,  0.5f, 0.0f}, {1.0, 1.0, 1.0}, {1.0, 1.0}},
        {{-0.5f,  0.5f, 0.0f}, {1.0, 1.0, 1.0}, {0.0, 1.0}},
    };
    
    constexpr NS::UInteger vertexBufferSize = 4 * sizeof(Vertex);
        
    constexpr uint16_t indices[] = {0, 1, 3, 2};
    constexpr NS::UInteger indexBufferSize = 4 * sizeof(ushort);
        
    //vertex buffer
    m_Mesh2D.m_VertexBuffer = device->newBuffer(vertexBufferSize, MTL::ResourceStorageModeShared);
    memcpy(m_Mesh2D.m_VertexBuffer->contents(), vertices, vertexBufferSize);
        
    //index buffer
    m_Mesh2D.m_IndexBuffer = device->newBuffer(indexBufferSize, MTL::ResourceStorageModeShared);
    memcpy(m_Mesh2D.m_IndexBuffer->contents(), indices, indexBufferSize);
    
    m_Mesh2D.m_Texture = new MetalTexture(textureFile, device);

    return m_Mesh2D;
}

Mesh_3D MeshBuilder::GeneratePlane(MTL::Device *device)
{
    constexpr Vertex3D vertices[] =
    {
        {{-0.5f, -0.5f, 0.5f}, {1.0, 1.0, 1.0}, {0.0, 0.0, 1.0}, {0.0, 1.0}}, // 0
        {{ 0.5f, -0.5f, 0.5f}, {1.0, 1.0, 1.0}, {0.0, 0.0, 1.0}, {1.0, 1.0}}, // 1
        {{ 0.5f,  0.5f, 0.5f}, {1.0, 1.0, 1.0}, {0.0, 0.0, 1.0}, {1.0, 0.0}}, // 2
        {{-0.5f,  0.5f, 0.5f}, {1.0, 1.0, 1.0}, {0.0, 0.0, 1.0}, {0.0, 0.0}}, // 3
    };
        
    constexpr uint16_t indices[] = {0, 1, 2, 2, 3, 0};
    
    m_Mesh3D.m_IndexCount = 6;
        
    //vertex buffer
    m_Mesh3D.m_VertexBuffer = MetalVertexBuffer::Create(device, sizeof(vertices));
    memcpy(m_Mesh3D.m_VertexBuffer->contents(), vertices, sizeof(vertices));
        
    //index buffer
    m_Mesh3D.m_IndexBuffer = MetalIndexBuffer::Create(device, indices, sizeof(indices));
    memcpy(m_Mesh3D.m_IndexBuffer->contents(), indices, sizeof(indices));
    
    return m_Mesh3D;
}

Mesh_3D MeshBuilder::GenerateCube(MTL::Device *device)
{
   
    constexpr Vertex3D vertices[] =
    {
        // Front face
        {{-0.5f, -0.5f, 0.5f}, {1.0, 1.0, 1.0}, {0.0, 0.0, 1.0}, {0.0, 1.0}}, // 0
        {{ 0.5f, -0.5f, 0.5f}, {1.0, 1.0, 1.0}, {0.0, 0.0, 1.0}, {1.0, 1.0}}, // 1
        {{ 0.5f,  0.5f, 0.5f}, {1.0, 1.0, 1.0}, {0.0, 0.0, 1.0}, {1.0, 0.0}}, // 2
        {{-0.5f,  0.5f, 0.5f}, {1.0, 1.0, 1.0}, {0.0, 0.0, 1.0}, {0.0, 0.0}}, // 3
        
        // Back face
        {{ 0.5f, -0.5f, -0.5f}, {1.0, 1.0, 1.0}, {0.0, 0.0, -1.0}, {0.0, 1.0}}, // 4
        {{-0.5f, -0.5f, -0.5f}, {1.0, 1.0, 1.0}, {0.0, 0.0, -1.0}, {1.0, 1.0}}, // 5
        {{-0.5f,  0.5f, -0.5f}, {1.0, 1.0, 1.0}, {0.0, 0.0, -1.0}, {1.0, 0.0}}, // 6
        {{ 0.5f,  0.5f, -0.5f}, {1.0, 1.0, 1.0}, {0.0, 0.0, -1.0}, {0.0, 0.0}}, // 7
        
        // Left face
        {{-0.5f, -0.5f, -0.5f}, {1.0, 1.0, 1.0}, {-1.0, 0.0, 0.0}, {0.0, 1.0}}, // 8
        {{-0.5f, -0.5f,  0.5f}, {1.0, 1.0, 1.0}, {-1.0, 0.0, 0.0}, {1.0, 1.0}}, // 9
        {{-0.5f,  0.5f,  0.5f}, {1.0, 1.0, 1.0}, {-1.0, 0.0, 0.0}, {1.0, 0.0}}, // 10
        {{-0.5f,  0.5f, -0.5f}, {1.0, 1.0, 1.0}, {-1.0, 0.0, 0.0}, {0.0, 0.0}}, // 11
        
        // Right face
        {{0.5f,  -0.5f,  0.5f}, {1.0, 1.0, 1.0}, {1.0, 0.0, 0.0}, {0.0, 1.0}}, // 12
        {{0.5f,  -0.5f, -0.5f}, {1.0, 1.0, 1.0}, {1.0, 0.0, 0.0}, {1.0, 1.0}}, // 13
        {{0.5f,   0.5f, -0.5f}, {1.0, 1.0, 1.0}, {1.0, 0.0, 0.0}, {1.0, 0.0}}, // 14
        {{0.5f,   0.5f,  0.5f}, {1.0, 1.0, 1.0}, {1.0, 0.0, 0.0}, {0.0, 0.0}}, // 15
        
        // Top face
        {{-0.5f,  0.5f,  0.5f}, {1.0, 1.0, 1.0}, {0.0, 1.0, 0.0}, {0.0, 1.0}}, // 16
        {{ 0.5f,  0.5f,  0.5f}, {1.0, 1.0, 1.0}, {0.0, 1.0, 0.0}, {1.0, 1.0}}, // 17
        {{ 0.5f,  0.5f, -0.5f}, {1.0, 1.0, 1.0}, {0.0, 1.0, 0.0}, {1.0, 0.0}}, // 18
        {{-0.5f,  0.5f, -0.5f}, {1.0, 1.0, 1.0}, {0.0, 1.0, 0.0}, {0.0, 0.0}}, // 19
        
        // Bottom face
        {{-0.5f, -0.5f, -0.5f}, {1.0, 1.0, 1.0}, {0.0, -1.0, 0.0},{0.0, 1.0}}, // 20
        {{ 0.5f, -0.5f, -0.5f}, {1.0, 1.0, 1.0}, {0.0, -1.0, 0.0},{1.0, 1.0}}, // 21
        {{ 0.5f, -0.5f,  0.5f}, {1.0, 1.0, 1.0}, {0.0, -1.0, 0.0},{1.0, 0.0}}, // 22
        {{-0.5f, -0.5f,  0.5f}, {1.0, 1.0, 1.0}, {0.0, -1.0, 0.0},{0.0, 0.0}}, // 23
       
    };
    
    constexpr uint16_t indices[] =
    {
       0, 1, 2, 2, 3, 0,
       4, 5, 6, 6, 7, 4,
       8, 9, 10, 10, 11, 8,
       12, 13, 14, 14, 15, 12,
       16, 17, 18, 18, 19, 16,
       20, 21, 22, 22, 23, 20
    };
    
    m_Mesh3D.m_IndexCount = 36;
    
    // vertex buffer
    m_Mesh3D.m_VertexBuffer = MetalVertexBuffer::Create(device, sizeof(vertices));
    memcpy(m_Mesh3D.m_VertexBuffer->contents(), vertices, sizeof(vertices));
    

    // Index buffer
    m_Mesh3D.m_IndexBuffer = MetalIndexBuffer::Create(device, indices, sizeof(indices));
    memcpy(m_Mesh3D.m_IndexBuffer->contents(), indices, sizeof(indices));
    
    return m_Mesh3D;
     
}

Mesh_3D MeshBuilder::GenerateSphere(MTL::Device* device, const int xSegments, const int ySegments, const simd::float3 &color)
{
    std::vector<Vertex3D> vertices;
    std::vector<u_int16_t> indices;
    
    constexpr float PI = 3.1415926539f;
    Vertex3D vertex;
    
    for (int x = 0; x <= xSegments; ++x)
    {
        for (int y = 0; y <= ySegments; ++y)
        {
            float xSegment = (float)x / (float)xSegments;
            float ySegment = (float)y / (float)ySegments;
            float xPos = simd::cos(xSegment * 2.0f * PI) * std::sin(ySegment * PI);
            float yPos = simd::cos(ySegment * PI);
            float zPos = simd::sin(xSegment * 2.0f * PI) * std::sin(ySegment * PI);

            
            vertex.pos = simd::make_float3(xPos, yPos, zPos);
            vertex.color = color;
            vertex.normals = simd::normalize(simd::make_float3(xPos, yPos, zPos));
            vertex.texCoord = simd::make_float2(xSegment, ySegment);
            
            vertices.push_back(vertex);
        }
    }
    
    
    for (int y = 0; y < ySegments; ++y)
    {
            for (int x = 0; x < xSegments; ++x)
            {
                uint16_t topLeft = y * (xSegments + 1) + x;
                uint16_t topRight = topLeft + 1;
                uint16_t bottomLeft = (y + 1) * (xSegments + 1) + x;
                uint16_t bottomRight = bottomLeft + 1;

                indices.push_back(topLeft);
                indices.push_back(bottomLeft);
                indices.push_back(topRight);
                
                indices.push_back(topRight);
                indices.push_back(bottomLeft);
                indices.push_back(bottomRight);
            }
    }
    
    
    m_Mesh3D.m_IndexCount = (u_int16_t)indices.size();
    
    size_t vertexSize = vertices.size() * sizeof(Vertex3D);
    size_t indexSize  = indices.size() * sizeof(uint16_t);
   
    // vertex buffer
    m_Mesh3D.m_VertexBuffer = MetalVertexBuffer::Create(device, static_cast<uint32_t>(vertexSize));
    memcpy(m_Mesh3D.m_VertexBuffer->contents(), vertices.data(), vertexSize);
    
    // Index buffer
    m_Mesh3D.m_IndexBuffer = MetalIndexBuffer::Create(device, indices.data(), static_cast<uint32_t>(indexSize));
    memcpy(m_Mesh3D.m_IndexBuffer->contents(), indices.data(), indexSize);
    
    return m_Mesh3D;
}
