//
//  MeshBuilder.hpp
//  LightningCore
//
//  Created by Kian Marvi on 5/6/25.
//

#ifndef MESHBUILDER_H
#define MESHBUILDER_H

#define HLSLPP_FEATURE_TRANSFORM
#include <hlsl++.h>

using hlslpp::float2;
using hlslpp::float3;

//#include "Sprite.h"
#include <vector>

using MeshHandle = size_t;

struct Vertex
{
    float3 m_Pos;
    float3 m_Color;
    float2 m_TexCoord;
};

struct Vertex3D
{
    float3 m_Pos;
    float3 m_Color;
    float3 m_Normals;
    float2 m_TexCoord;
    float3 m_Tangent;
    float3 m_Binormal;
};

/*
struct Mesh_2D
{
    Sprite   m_Sprite;
    float4x4 m_Transform;
};
*/
struct Mesh_3D
{
    std::vector<Vertex3D> m_Vertices;
    std::vector<uint16_t> m_Indices;
    size_t                m_VertexSize;
    size_t                m_IndexSize;
    uint16_t              m_IndexCount;
};

class MeshBuilder
{
public:
    MeshBuilder() = default;
    ~MeshBuilder(){};
    
    // Platform agnotic implementation
    static Mesh_3D GeneratePlane();
    static Mesh_3D GenerateCube();
    static Mesh_3D GenerateSphere(const int xSegments, const int ySegments, const float3 color);
};
#endif /* MESHBUILDER_H */
