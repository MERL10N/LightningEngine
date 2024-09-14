//
//  Mesh.h
//  LightningEngineCore
//
//  Created by Kian Marvi on 9/13/24.
//

#ifndef Mesh_hpp
#define Mesh_hpp

#include <simd/simd.h>
#include <vector>
#include <stdint.h>

// Forward declaration of classes in the MTL namespce within the metal-cpp framework
namespace MTL
{
  class Device;
  class Buffer;
}


struct Vertex
{
    simd::float3 position;
    simd::float3 normal;
    simd::float4 color;
    simd::float2 texCoord;
    
    Vertex()
    {
        position = simd::float3();
        normal = simd_make_float3(0.0f, 0.0f, 1.0f);
        color = simd_make_float4(1.0f);
        texCoord = simd::float2();
    }
};


class Mesh
{
public:
    enum DRAW_MODE
    {
        DRAW_TRIANGLES,
        DRAW_TRIANGLES_ARRAY,
        DRAW_TRIANGLE_STRIP,
        DRAW_LINES,
        DRAW_TRIANGLES_INSTANCED,
        DRAW_MODE_LAST,
    };
    
    MTL::Buffer* vertexBuffer;
    MTL::Buffer* indexBuffer;
    unsigned long indexSize;
    DRAW_MODE mode;
    
    Mesh();
    Mesh(std::vector<Vertex>& vertices, std::vector<uint32_t>& indices, MTL::Device* metalDevice);
    ~Mesh();
    
private:
    MTL::Device* metalDevice;
    std::vector<Vertex> vertices;
    std::vector<uint32_t> indices;

    uint16_t vertexSize;
    
    
};


#endif /* Mesh_hpp */
