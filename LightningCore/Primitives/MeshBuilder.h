//
//  MeshBuilder.hpp
//  LightningCore
//
//  Created by Kian Marvi on 5/6/25.
//

#ifndef MESHBUILDER_H
#define MESHBUILDER_H

#include <simd/simd.h>
#include "Sprite.h"

class MetalTexture;
namespace MTL
{
   class Device;
   class Buffer;
}

struct Vertex
{
    simd::float3 pos;
    simd::float3 color;
    simd::float2 texCoord;
};

struct Vertex3D
{
    simd::float3 pos;
    simd::float3 color;
    simd::float3 normals;
    simd::float2 texCoord;
};

struct Mesh_2D
{
    MTL::Buffer* m_VertexBuffer, *m_IndexBuffer;
    MetalTexture* m_Texture;
    Sprite m_Sprite;
    matrix_float4x4 m_Transform;
};

struct Mesh_3D
{
    MTL::Buffer* m_VertexBuffer, *m_IndexBuffer;
    MTL::Buffer* m_ArgumentBuffer;
    //MetalTexture* m_Texture;
    uint16_t m_IndexCount;
};

class MeshBuilder
{
public:
    MeshBuilder() = default;
    ~MeshBuilder(){};
    Mesh_2D GenerateQuadWithTexture(MTL::Device* device, const char* texture);
    Mesh_2D GenerateQuad(MTL::Device* device);
    Mesh_3D GenerateCube(MTL::Device* device);
    Mesh_3D GenerateSphere(MTL::Device* device, const int xSegments, const int ySegments);
    Mesh_3D GenerateSphere(MTL::Device* device, const int xSegments, const int ySegments, const simd::float3 &color);
private:
    Mesh_2D m_Mesh2D;
    Mesh_3D m_Mesh3D;
};
#endif /* MESHBUILDER_H */
