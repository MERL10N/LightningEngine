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
using namespace hlslpp;

#include "Sprite.h"
#include <stddef.h>
#include <vector>

class MetalTexture;
namespace MTL
{
   class Device;
   class Buffer;
}

struct Vertex
{
    float3 pos;
    float3 color;
    float2 texCoord;
};

struct Vertex3D
{
    float3 pos;
    float3 color;
    float3 normals;
    float2 texCoord;
};

struct Mesh_2D
{
    MTL::Buffer* m_VertexBuffer, *m_IndexBuffer;
    MetalTexture* m_Texture;
    Sprite m_Sprite;
    float4x4 m_Transform;
};

struct Mesh_3D
{
    MTL::Buffer* m_VertexBuffer, *m_IndexBuffer;
    
    // Platform agnostic member variables
    std::vector<Vertex3D> m_Vertices;
    std::vector<uint16_t> m_Indices;
    size_t                m_VertexSize = 0;
    size_t                m_IndexSize  = 0;
    uint16_t              m_IndexCount = 0;
    bool                  b_Instanced = false;
};

class MeshBuilder
{
public:
    MeshBuilder() = default;
    ~MeshBuilder(){};
    Mesh_2D GenerateQuadWithTexture(MTL::Device* device, const char* texture);
    Mesh_3D GeneratePlane(MTL::Device* device);
    Mesh_3D GenerateCube(MTL::Device* device);
    Mesh_3D GenerateSphere(MTL::Device* device, const int xSegments, const int ySegments, const float3 &color = float3(0.5f, 0.5f, 0.5f));
    
    // Platform agnotic implementation
    Mesh_3D GenerateCube();
    Mesh_3D GenerateSphere(const int xSegments, const int ySegments, const float3 &color = float3(0.5f, 0.5f, 0.5f));
    
    const std::vector<Vertex3D>& GetVertices() const { return m_Mesh3D.m_Vertices; }
    const std::vector<uint16_t>& GetIndices()  const { return m_Mesh3D.m_Indices;  }
private:
    Mesh_2D m_Mesh2D;
    Mesh_3D m_Mesh3D;
};
#endif /* MESHBUILDER_H */
