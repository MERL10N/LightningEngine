//
//  MeshBuilder.cpp
//  LightningCore
//
//  Created by Kian Marvi on 5/6/25.
//

#include "MeshBuilder.h"

Mesh_3D MeshBuilder::GeneratePlane()
{
    Mesh_3D mesh;
    
    mesh.m_Vertices =
    {
        {{-0.5f, -0.5f, 0.5f}, {1.0, 1.0, 1.0}, {0.0, 0.0, 1.0}, {0.0, 1.0}}, // 0
        {{ 0.5f, -0.5f, 0.5f}, {1.0, 1.0, 1.0}, {0.0, 0.0, 1.0}, {1.0, 1.0}}, // 1
        {{ 0.5f,  0.5f, 0.5f}, {1.0, 1.0, 1.0}, {0.0, 0.0, 1.0}, {1.0, 0.0}}, // 2
        {{-0.5f,  0.5f, 0.5f}, {1.0, 1.0, 1.0}, {0.0, 0.0, 1.0}, {0.0, 0.0}}, // 3
    };
        
    mesh.m_Indices = {0, 1, 2, 2, 3, 0};
    
    mesh.m_IndexCount = sizeof(mesh.m_Indices);

    mesh.m_VertexSize = mesh.m_Vertices.size() * sizeof(Vertex3D);
    mesh.m_IndexSize  = mesh.m_Indices.size()  * sizeof(uint16_t);
    
    return mesh;
}

Mesh_3D MeshBuilder::GenerateCube()
{
    Mesh_3D mesh;
    
    mesh.m_Vertices =
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
    
    mesh.m_Indices =
    {
       0, 1, 2, 2, 3, 0,
       4, 5, 6, 6, 7, 4,
       8, 9, 10, 10, 11, 8,
       12, 13, 14, 14, 15, 12,
       16, 17, 18, 18, 19, 16,
       20, 21, 22, 22, 23, 20
    };
    
    mesh.m_IndexCount = 36;
    
    mesh.m_VertexSize = mesh.m_Vertices.size() * sizeof(Vertex3D);
    mesh.m_IndexSize  = mesh.m_Indices.size()  * sizeof(uint16_t);
    
    return mesh;
     
}
Mesh_3D MeshBuilder::GenerateSphere(const int xSegments, const int ySegments, const float3 color)
{
    Mesh_3D mesh;
    
    constexpr float PI = 3.1415926539f;
    Vertex3D vertex;
    
    for (int x = 0; x <= xSegments; ++x)
    {
        for (int y = 0; y <= ySegments; ++y)
        {
            float xSegment = (float)x / (float)xSegments;
            float ySegment = (float)y / (float)ySegments;
            float xPos = cos(xSegment * 2.0f * PI) * sin(ySegment * PI);
            float yPos = cos(ySegment * PI);
            float zPos = sin(xSegment * 2.0f * PI) * sin(ySegment * PI);

            
            vertex.m_Pos = float3(xPos, yPos, zPos);
            vertex.m_Color = color;
            vertex.m_Normals = normalize(float3(xPos, yPos, zPos));
            vertex.m_TexCoord = float2(xSegment, ySegment);
            
            mesh.m_Vertices.emplace_back(vertex);
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

                mesh.m_Indices.emplace_back(topLeft);
                mesh.m_Indices.emplace_back(bottomLeft);
                mesh.m_Indices.emplace_back(topRight);
                
                mesh.m_Indices.emplace_back(topRight);
                mesh.m_Indices.emplace_back(bottomLeft);
                mesh.m_Indices.emplace_back(bottomRight);
            }
    }
    
    
    mesh.m_IndexCount = static_cast<u_int16_t>(mesh.m_Indices.size());
    
    mesh.m_VertexSize = mesh.m_Vertices.size() * sizeof(Vertex3D);
    mesh.m_IndexSize  = mesh.m_Indices.size()  * sizeof(uint16_t);
    
    return mesh;
}

