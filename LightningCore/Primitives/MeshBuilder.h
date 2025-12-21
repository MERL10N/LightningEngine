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
    simd::float3 pos;   //(x,y,z)
    simd::float3 color; //(r,g,b)
    simd::float2 texCoord; //(u,v)
};

struct Mesh
{
    MTL::Buffer* vertexBuffer, *indexBuffer;
    MTL::VertexDescriptor* vertexDescriptor;
    MetalTexture* texture;
};

class MeshBuilder
{
public:
    MeshBuilder() = default;
    ~MeshBuilder();
    Mesh GenerateQuad(MTL::Device* device, const char* texture);
private:
    Mesh m_Mesh;
};
#endif /* MESHBUILDER_H */
