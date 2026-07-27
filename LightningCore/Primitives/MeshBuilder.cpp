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
    
    // positions
    float3 pos1(-1.0f,  1.0f, 0.0f);
    float3 pos2(-1.0f, -1.0f, 0.0f);
    float3 pos3( 1.0f, -1.0f, 0.0f);
    float3 pos4( 1.0f,  1.0f, 0.0f);

    // color
    float3 color(1.0f, 1.0f, 1.0f);
    
    // texture coordinates
    float2 uv1(0.0f, 1.0f);
    float2 uv2(0.0f, 0.0f);
    float2 uv3(1.0f, 0.0f);
    float2 uv4(1.0f, 1.0f);
    // normal vector
    float3 normal(0.0f, 0.0f, 1.0f);

    // calculate tangent/bitangent vectors of both triangles
    float3 tangent1, bitangent1;
    float3 tangent2, bitangent2;
    // triangle 1
    // ----------
    float3 edge1 = pos2 - pos1;
    float3 edge2 = pos3 - pos1;
    float2 deltaUV1 = uv2 - uv1;
    float2 deltaUV2 = uv3 - uv1;

    float f = 1.0f / (deltaUV1.x * deltaUV2.y - deltaUV2.x * deltaUV1.y);

    tangent1.x = f * (deltaUV2.y * edge1.x - deltaUV1.y * edge2.x);
    tangent1.y = f * (deltaUV2.y * edge1.y - deltaUV1.y * edge2.y);
    tangent1.z = f * (deltaUV2.y * edge1.z - deltaUV1.y * edge2.z);

    bitangent1.x = f * (-deltaUV2.x * edge1.x + deltaUV1.x * edge2.x);
    bitangent1.y = f * (-deltaUV2.x * edge1.y + deltaUV1.x * edge2.y);
    bitangent1.z = f * (-deltaUV2.x * edge1.z + deltaUV1.x * edge2.z);

    // triangle 2
    // ----------
    edge1 = pos3 - pos1;
    edge2 = pos4 - pos1;
    deltaUV1 = uv3 - uv1;
    deltaUV2 = uv4 - uv1;

    f = 1.0f / (deltaUV1.x * deltaUV2.y - deltaUV2.x * deltaUV1.y);

    tangent2.x = f * (deltaUV2.y * edge1.x - deltaUV1.y * edge2.x);
    tangent2.y = f * (deltaUV2.y * edge1.y - deltaUV1.y * edge2.y);
    tangent2.z = f * (deltaUV2.y * edge1.z - deltaUV1.y * edge2.z);


    bitangent2.x = f * (-deltaUV2.x * edge1.x + deltaUV1.x * edge2.x);
    bitangent2.y = f * (-deltaUV2.x * edge1.y + deltaUV1.x * edge2.y);
    bitangent2.z = f * (-deltaUV2.x * edge1.z + deltaUV1.x * edge2.z);
    
    
    mesh.m_Vertices =
    {
        {pos1, color, normal, uv1, tangent1, bitangent1}, // 0
        {pos2, color, normal, uv2, tangent1, bitangent1}, // 1
        {pos3, color, normal, uv3, tangent2, bitangent2}, // 2
        {pos4, color, normal, uv4, tangent2, bitangent2}, // 3
    };
        
    mesh.m_Indices = {0, 1, 2, 2, 3, 0};
    
    mesh.m_IndexCount = 6;

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

            
            vertex.m_Pos       = float3(xPos, yPos, zPos);
            vertex.m_Color     = color;
            vertex.m_Normals   = normalize(float3(xPos, yPos, zPos));
            vertex.m_TexCoord  = float2(xSegment, ySegment);
            vertex.m_Tangent   = float3(0.0f, 0.0f, 0.0f);
            vertex.m_Bitangent = float3(0.0f, 0.0f, 0.0f);
            
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

