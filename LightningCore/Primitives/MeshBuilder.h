//
//  MeshBuilder.hpp
//  LightningCore
//
//  Created by Kian Marvi on 5/6/25.
//

#ifndef MESHBUILDER_H
#define MESHBUILDER_H

#include <simd/simd.h>
class MetalVertexBuffer;
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

struct Uniforms
{
    simd::float4x4 model, view, projection;
};

struct Mesh
{
    MTL::Buffer* vertexBuffer, *indexBuffer, *transformationBuffer;
    MetalTexture* texture;
};

class MeshBuilder
{
public:
    MeshBuilder() = default;
    ~MeshBuilder();
    Mesh GenerateQuad(MTL::Device* device, const char* texture);
    Mesh GenerateCube(MTL::Device* device, const char* texture);
private:
    Mesh m_Mesh;
};
#endif /* MESHBUILDER_H */
