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
    MetalTexture* m_Texture;
    matrix_float4x4 m_Transform, m_Rotation;
};

class MeshBuilder
{
public:
    MeshBuilder() = default;
    ~MeshBuilder(){};
    Mesh_2D GenerateQuadWithTexture(MTL::Device* device, const char* texture);
    Mesh_2D GenerateQuad(MTL::Device* device);
    Mesh_3D GenerateCube(MTL::Device* device, const char* texture);
private:
    Mesh_2D m_Mesh2D;
    Mesh_3D m_Mesh3D;
};
#endif /* MESHBUILDER_H */
