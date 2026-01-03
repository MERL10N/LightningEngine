//
//  MeshBuilder.hpp
//  LightningCore
//
//  Created by Kian Marvi on 5/6/25.
//

#ifndef MESHBUILDER_H
#define MESHBUILDER_H

#include <simd/simd.h>
class MetalTexture;
namespace MTL
{
   class Device;
   class Buffer;
   class Texture;
   class VertexDescriptor;
}

struct Vertex
{
    simd::float3 pos;
    simd::float3 color;
    simd::float2 texCoord; 
};

struct Mesh
{
    MTL::Buffer* m_VertexBuffer, *m_IndexBuffer;
    MetalTexture* m_Texture;
    matrix_float4x4 m_Transform;
};

class MeshBuilder
{
public:
    MeshBuilder() = default;
    ~MeshBuilder(){};
    Mesh GenerateQuadWithTexture(MTL::Device* device, const char* texture);
    Mesh GenerateQuad(MTL::Device* device);
    Mesh GenerateCube(MTL::Device* device, const char* texture);
private:
    Mesh m_Mesh;
};
#endif /* MESHBUILDER_H */
