//
//  Mesh.h
//  Metal-Tutorial
//
//  Created by Kian Marvi on 24/1/24.
//

#ifndef Mesh_h
#define Mesh_h

#include <simd/simd.h>
#include <Metal/Metal.hpp>

using namespace simd;
/**
    Vertex Class
    Used for 2D texture.
    Create a version with normals for 3D normal
*/
struct Vertex
{
    float3 position;
    float3 normal;
    float4 colour;
    float2 texCoord;
    Vertex()
    {
        position = float3();
        normal = make_float3(0.0f, 0.0f, 1.0f);
        colour = float4(1.0);
        texCoord = float2();
    }
};

/**
    Mesh
    Used for generating mesh for rendering
    Contains the vertex buffer and index buffer
*/
class CMesh
{
public:
    enum DRAW_MODE
    {
        DRAW_TRIANGLES, //default mode
        DRAW_TRIANGLE_STRIP,
        DRAW_LINES,
        DRAW_MODE_LAST,
    };

    MTL::Device* device;
    MTL::Buffer* vertexBuffer;
    MTL::Buffer* indexBuffer;
    NS::UInteger indexSize;

    DRAW_MODE mode;

    // Constructor
    CMesh(MTL::Device* device);
    // Destructor
    ~CMesh(void);
    virtual void Render(MTL::RenderCommandEncoder* renderCommandEncoder);
};

#endif /* Mesh_h */
