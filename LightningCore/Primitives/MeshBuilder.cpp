//
//  MeshBuilder.cpp
//  LightningCore
//
//  Created by Kian Marvi on 5/6/25.
//

#include "MeshBuilder.h"
#include <hlsl++/vector_float.h>
#include <math.h>

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
    
    // -- vertex positions
    // Front
    float3 pos1(-0.5f, -0.5f, 0.5f);
    float3 pos2( 0.5f, -0.5f, 0.5f);
    float3 pos3( 0.5f,  0.5f, 0.5f);
    float3 pos4(-0.5f,  0.5f, 0.5f);
    
    // Back
    float3 pos5( 0.5f, -0.5f, -0.5f);
    float3 pos6(-0.5f, -0.5f, -0.5f);
    float3 pos7(-0.5f,  0.5f, -0.5f);
    float3 pos8( 0.5f,  0.5f, -0.5f);
    
    // Left
    float3 pos9 (-0.5f, -0.5f, -0.5f);
    float3 pos10(-0.5f, -0.5f,  0.5f);
    float3 pos11(-0.5f,  0.5f,  0.5f);
    float3 pos12(-0.5f,  0.5f, -0.5f);
    
    // Right
    float3 pos13(0.5f,  -0.5f,  0.5f);
    float3 pos14(0.5f,  -0.5f, -0.5f);
    float3 pos15(0.5f,   0.5f, -0.5f);
    float3 pos16(0.5f,   0.5f,  0.5f);
    
    // Top
    float3 pos17(-0.5f,  0.5f,  0.5f);
    float3 pos18( 0.5f,  0.5f,  0.5f);
    float3 pos19( 0.5f,  0.5f, -0.5f);
    float3 pos20(-0.5f,  0.5f, -0.5f);
    
    // Bottom
    float3 pos21(-0.5f, -0.5f, -0.5f);
    float3 pos22( 0.5f, -0.5f, -0.5f);
    float3 pos23( 0.5f, -0.5f,  0.5f);
    float3 pos24(-0.5f, -0.5f,  0.5f);

    // -- color
    float3 color(1.0f, 1.0f, 1.0f);
    
    // texture coordinates
    float2 uv1(0.0f, 1.0f);
    float2 uv2(1.0f, 1.0f);
    float2 uv3(1.0f, 0.0f);
    float2 uv4(0.0f, 0.0f);
    
    // -- normal vector
    float3 normal1(0.0f, 0.0f, 1.0f);
    float3 normal2(0.0f, 0.0f, -1.0f);
    float3 normal3(-1.0f, 0.0f, 0.0f);
    float3 normal4(1.0f, 0.0f, 0.0f);
    float3 normal5(0.0f, 1.0f, 0.0f);
    float3 normal6(0.0f, -1.0f, 0.0f);
    
    // calculate tangent/bitangent vectors of both triangles
    float3 tangent1, bitangent1;
    float3 tangent2, bitangent2;
    float3 tangent3, bitangent3;
    float3 tangent4, bitangent4;
    float3 tangent5, bitangent5;
    float3 tangent6, bitangent6;
    float3 tangent7, bitangent7;
    float3 tangent8, bitangent8;
    float3 tangent9, bitangent9;
    float3 tangent10, bitangent10;
    float3 tangent11, bitangent11;
    float3 tangent12, bitangent12;
    
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
    tangent1   = normalize(tangent1);

    bitangent1.x = f * (-deltaUV2.x * edge1.x + deltaUV1.x * edge2.x);
    bitangent1.y = f * (-deltaUV2.x * edge1.y + deltaUV1.x * edge2.y);
    bitangent1.z = f * (-deltaUV2.x * edge1.z + deltaUV1.x * edge2.z);
    bitangent1    = normalize(bitangent1);

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
    tangent2   = normalize(tangent2);


    bitangent2.x = f * (-deltaUV2.x * edge1.x + deltaUV1.x * edge2.x);
    bitangent2.y = f * (-deltaUV2.x * edge1.y + deltaUV1.x * edge2.y);
    bitangent2.z = f * (-deltaUV2.x * edge1.z + deltaUV1.x * edge2.z);
    bitangent2   = normalize(bitangent2);
    
    // triangle 3
    // ----------
    edge1 = pos6 - pos5;
    edge2 = pos7 - pos5;
    deltaUV1 = uv2 - uv1;
    deltaUV2 = uv3 - uv1;

    f = 1.0f / (deltaUV1.x * deltaUV2.y - deltaUV2.x * deltaUV1.y);

    tangent3.x = f * (deltaUV2.y * edge1.x - deltaUV1.y * edge2.x);
    tangent3.y = f * (deltaUV2.y * edge1.y - deltaUV1.y * edge2.y);
    tangent3.z = f * (deltaUV2.y * edge1.z - deltaUV1.y * edge2.z);
    tangent3   = normalize(tangent3);
    

    bitangent3.x = f * (-deltaUV2.x * edge1.x + deltaUV1.x * edge2.x);
    bitangent3.y = f * (-deltaUV2.x * edge1.y + deltaUV1.x * edge2.y);
    bitangent3.z = f * (-deltaUV2.x * edge1.z + deltaUV1.x * edge2.z);
    bitangent3   = normalize(bitangent3);
    
    // triangle 4
    // ----------
    edge1 = pos7 - pos5;
    edge2 = pos8 - pos5;
    deltaUV1 = uv3 - uv1;
    deltaUV2 = uv4 - uv1;

    f = 1.0f / (deltaUV1.x * deltaUV2.y - deltaUV2.x * deltaUV1.y);

    tangent4.x = f * (deltaUV2.y * edge1.x - deltaUV1.y * edge2.x);
    tangent4.y = f * (deltaUV2.y * edge1.y - deltaUV1.y * edge2.y);
    tangent4.z = f * (deltaUV2.y * edge1.z - deltaUV1.y * edge2.z);
    tangent4   = normalize(tangent4);

    bitangent4.x = f * (-deltaUV2.x * edge1.x + deltaUV1.x * edge2.x);
    bitangent4.y = f * (-deltaUV2.x * edge1.y + deltaUV1.x * edge2.y);
    bitangent4.z = f * (-deltaUV2.x * edge1.z + deltaUV1.x * edge2.z);
    bitangent4  = normalize(bitangent4);
    
    // triangle 5
    // ----------
    edge1 = pos10 - pos9;
    edge2 = pos11 - pos9;
    deltaUV1 = uv2 - uv1;
    deltaUV2 = uv3 - uv1;

    f = 1.0f / (deltaUV1.x * deltaUV2.y - deltaUV2.x * deltaUV1.y);

    tangent5.x = f * (deltaUV2.y * edge1.x - deltaUV1.y * edge2.x);
    tangent5.y = f * (deltaUV2.y * edge1.y - deltaUV1.y * edge2.y);
    tangent5.z = f * (deltaUV2.y * edge1.z - deltaUV1.y * edge2.z);
    tangent5   = normalize(tangent5);

    bitangent5.x = f * (-deltaUV2.x * edge1.x + deltaUV1.x * edge2.x);
    bitangent5.y = f * (-deltaUV2.x * edge1.y + deltaUV1.x * edge2.y);
    bitangent5.z = f * (-deltaUV2.x * edge1.z + deltaUV1.x * edge2.z);
    bitangent5   = normalize(bitangent5);
    
    // triangle 6
    // ----------
    edge1 = pos11 - pos9;
    edge2 = pos12 - pos9;
    deltaUV1 = uv3 - uv1;
    deltaUV2 = uv4 - uv1;

    f = 1.0f / (deltaUV1.x * deltaUV2.y - deltaUV2.x * deltaUV1.y);

    tangent6.x = f * (deltaUV2.y * edge1.x - deltaUV1.y * edge2.x);
    tangent6.y = f * (deltaUV2.y * edge1.y - deltaUV1.y * edge2.y);
    tangent6.z = f * (deltaUV2.y * edge1.z - deltaUV1.y * edge2.z);
    tangent6   = normalize(tangent6);

    bitangent6.x = f * (-deltaUV2.x * edge1.x + deltaUV1.x * edge2.x);
    bitangent6.y = f * (-deltaUV2.x * edge1.y + deltaUV1.x * edge2.y);
    bitangent6.z = f * (-deltaUV2.x * edge1.z + deltaUV1.x * edge2.z);
    bitangent6   = normalize(bitangent6);
    // triangle 7
    // ----------
    edge1 = pos14 - pos13;
    edge2 = pos15 - pos13;
    deltaUV1 = uv2 - uv1;
    deltaUV2 = uv3 - uv1;

    f = 1.0f / (deltaUV1.x * deltaUV2.y - deltaUV2.x * deltaUV1.y);

    tangent7.x = f * (deltaUV2.y * edge1.x - deltaUV1.y * edge2.x);
    tangent7.y = f * (deltaUV2.y * edge1.y - deltaUV1.y * edge2.y);
    tangent7.z = f * (deltaUV2.y * edge1.z - deltaUV1.y * edge2.z);
    tangent7   = normalize(tangent7);

    bitangent7.x = f * (-deltaUV2.x * edge1.x + deltaUV1.x * edge2.x);
    bitangent7.y = f * (-deltaUV2.x * edge1.y + deltaUV1.x * edge2.y);
    bitangent7.z = f * (-deltaUV2.x * edge1.z + deltaUV1.x * edge2.z);
    bitangent7   = normalize(bitangent7);
    
    // triangle 8
    // ----------
    edge1 = pos15 - pos13;
    edge2 = pos16 - pos13;
    deltaUV1 = uv3 - uv1;
    deltaUV2 = uv4 - uv1;

    f = 1.0f / (deltaUV1.x * deltaUV2.y - deltaUV2.x * deltaUV1.y);

    tangent8.x = f * (deltaUV2.y * edge1.x - deltaUV1.y * edge2.x);
    tangent8.y = f * (deltaUV2.y * edge1.y - deltaUV1.y * edge2.y);
    tangent8.z = f * (deltaUV2.y * edge1.z - deltaUV1.y * edge2.z);
    tangent8   = normalize(tangent8);

    bitangent8.x = f * (-deltaUV2.x * edge1.x + deltaUV1.x * edge2.x);
    bitangent8.y = f * (-deltaUV2.x * edge1.y + deltaUV1.x * edge2.y);
    bitangent8.z = f * (-deltaUV2.x * edge1.z + deltaUV1.x * edge2.z);
    bitangent8   = normalize(bitangent8);
    
    // triangle 9
    // ----------
    edge1 = pos18 - pos17;
    edge2 = pos19 - pos17;
    deltaUV1 = uv2 - uv1;
    deltaUV2 = uv3 - uv1;

    f = 1.0f / (deltaUV1.x * deltaUV2.y - deltaUV2.x * deltaUV1.y);

    tangent9.x = f * (deltaUV2.y * edge1.x - deltaUV1.y * edge2.x);
    tangent9.y = f * (deltaUV2.y * edge1.y - deltaUV1.y * edge2.y);
    tangent9.z = f * (deltaUV2.y * edge1.z - deltaUV1.y * edge2.z);
    tangent9   = normalize(tangent9);

    bitangent9.x = f * (-deltaUV2.x * edge1.x + deltaUV1.x * edge2.x);
    bitangent9.y = f * (-deltaUV2.x * edge1.y + deltaUV1.x * edge2.y);
    bitangent9.z = f * (-deltaUV2.x * edge1.z + deltaUV1.x * edge2.z);
    bitangent9   = normalize(bitangent9);
    
    // triangle 10
    // ----------
    edge1 = pos19 - pos17;
    edge2 = pos20 - pos17;
    deltaUV1 = uv3 - uv1;
    deltaUV2 = uv4 - uv1;

    f = 1.0f / (deltaUV1.x * deltaUV2.y - deltaUV2.x * deltaUV1.y);

    tangent10.x = f * (deltaUV2.y * edge1.x - deltaUV1.y * edge2.x);
    tangent10.y = f * (deltaUV2.y * edge1.y - deltaUV1.y * edge2.y);
    tangent10.z = f * (deltaUV2.y * edge1.z - deltaUV1.y * edge2.z);
    tangent10   = normalize(tangent10);

    bitangent10.x = f * (-deltaUV2.x * edge1.x + deltaUV1.x * edge2.x);
    bitangent10.y = f * (-deltaUV2.x * edge1.y + deltaUV1.x * edge2.y);
    bitangent10.z = f * (-deltaUV2.x * edge1.z + deltaUV1.x * edge2.z);
    bitangent10   = normalize(bitangent10);
    
    // triangle 11
    // ----------
    edge1 = pos22 - pos21;
    edge2 = pos23 - pos21;
    deltaUV1 = uv2 - uv1;
    deltaUV2 = uv3 - uv1;

    f = 1.0f / (deltaUV1.x * deltaUV2.y - deltaUV2.x * deltaUV1.y);

    tangent11.x = f * (deltaUV2.y * edge1.x - deltaUV1.y * edge2.x);
    tangent11.y = f * (deltaUV2.y * edge1.y - deltaUV1.y * edge2.y);
    tangent11.z = f * (deltaUV2.y * edge1.z - deltaUV1.y * edge2.z);
    tangent11   = normalize(tangent11);

    bitangent11.x = f * (-deltaUV2.x * edge1.x + deltaUV1.x * edge2.x);
    bitangent11.y = f * (-deltaUV2.x * edge1.y + deltaUV1.x * edge2.y);
    bitangent11.z = f * (-deltaUV2.x * edge1.z + deltaUV1.x * edge2.z);
    bitangent11   = normalize(bitangent11);
    
    // triangle 12
    // ----------
    edge1 = pos23 - pos21;
    edge2 = pos24 - pos21;
    deltaUV1 = uv3 - uv1;
    deltaUV2 = uv4 - uv1;

    f = 1.0f / (deltaUV1.x * deltaUV2.y - deltaUV2.x * deltaUV1.y);

    tangent12.x = f * (deltaUV2.y * edge1.x - deltaUV1.y * edge2.x);
    tangent12.y = f * (deltaUV2.y * edge1.y - deltaUV1.y * edge2.y);
    tangent12.z = f * (deltaUV2.y * edge1.z - deltaUV1.y * edge2.z);
    tangent12   = normalize(tangent12);

    bitangent12.x = f * (-deltaUV2.x * edge1.x + deltaUV1.x * edge2.x);
    bitangent12.y = f * (-deltaUV2.x * edge1.y + deltaUV1.x * edge2.y);
    bitangent12.z = f * (-deltaUV2.x * edge1.z + deltaUV1.x * edge2.z);
    bitangent12   = normalize(bitangent12);
    
    mesh.m_Vertices =
    {
        // Front face
        {pos1, color, normal1, uv1, tangent1, bitangent1},
        {pos2, color, normal1, uv2, tangent1, bitangent1},
        {pos3, color, normal1, uv3, tangent2, bitangent2},
        {pos4, color, normal1, uv4, tangent2, bitangent2},
        
        // Back face
        {pos5, color, normal2, uv1, tangent3, bitangent3},
        {pos6, color, normal2, uv2, tangent3, bitangent3},
        {pos7, color, normal2, uv3, tangent4, bitangent4},
        {pos8, color, normal2, uv4, tangent4, bitangent4},
        
        // Left face
        {pos9, color, normal3, uv1, tangent5, bitangent5},
        {pos10, color, normal3, uv2, tangent5, bitangent5},
        {pos11, color, normal3, uv3, tangent6, bitangent6},
        {pos12, color, normal3, uv4, tangent6, bitangent6},
        
        // Right face
        {pos13, color, normal4, uv1, tangent7, bitangent7},
        {pos14, color, normal4, uv2, tangent7, bitangent7},
        {pos15, color, normal4, uv3, tangent8, bitangent8},
        {pos16, color, normal4, uv4, tangent8, bitangent8},
        
        // Top Face
        {pos17, color, normal5, uv1, tangent9, bitangent9},
        {pos18, color, normal5, uv2, tangent9, bitangent9},
        {pos19, color, normal5, uv3, tangent10, bitangent10},
        {pos20, color, normal5, uv4, tangent10, bitangent10},
        
        // Bottom Face
        {pos21, color, normal6, uv1, tangent11, bitangent11},
        {pos22, color, normal6, uv2, tangent11, bitangent11},
        {pos23, color, normal6, uv3, tangent12, bitangent12},
        {pos24, color, normal6, uv4, tangent12, bitangent12},
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
    
    mesh.m_IndexCount = mesh.m_Indices.size();
    
    mesh.m_VertexSize = mesh.m_Vertices.size() * sizeof(Vertex3D);
    mesh.m_IndexSize  = mesh.m_IndexCount  * sizeof(uint16_t);
    
    return mesh;
     
}

Mesh_3D MeshBuilder::GenerateSphere(const int xSegments, const int ySegments, const float3 &color)
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
            
            float theta = xSegment * 2.0f * PI;
            float phi   = ySegment * PI;
            float xPos = cos(theta) * sin(phi);
            float yPos = cos(phi);
            float zPos = sin(theta) * sin(phi);
            
            vertex.m_Pos       = float3(xPos, yPos, zPos);
            vertex.m_Color     = color;
            vertex.m_Normals   = normalize(float3(xPos, yPos, zPos));  // Normalize the position vector of the sphere to get the normal vector
            vertex.m_TexCoord  = float2(xSegment, ySegment);
            vertex.m_Tangent   = float3(sin(theta), 0, cos(theta));   // Unit tangent vector that is a partial derivative with respect to theta
            vertex.m_Binormal  = cross(vertex.m_Tangent, vertex.m_Normals);
            
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
