/*
    MeshBuilder.cpp
    LightningEngine

    Created by Kian Marvi on 2/2/24.
*/

#include "MeshBuilder.h"
#include "VertexData.h"

#include <System/ImageLoader.h>

/**
 @brief Generate a quad and load it into Metal
 @param colour A simd::float4 variable containing the colour of this quad
 @param width A const float variable containing the width of the quad
 @param height A const float variable containing the height of the quad
 @param bCentralisedOrigin A bool variable to decide if the quad is centralised around the origin
 @param metalDevice A metal compatible device (Either mac or iOS)
 */

CMesh* CMeshBuilder::GenerateQuad(simd::float4 colour, const float width, const float height, MTL::Device* metalDevice, bool bCentralisedOrigin)
{
    Vertex v;
    std::vector<Vertex> vertex_buffer_data;
    std::vector<NS::UInteger> index_buffer_data;
    
    if (bCentralisedOrigin == true)
            v.position = make_float3(-0.5f * width, -0.5f * height, 0);
    else
            v.position = make_float3(0.0f, 0.0f, 0.f);
    
    v.normal = make_float3(0.0f, 0.0f, 1.0f);
    v.colour = colour;
    v.texCoord = make_float2(0.f, 0.f);
    vertex_buffer_data.push_back(v);

    if (bCentralisedOrigin == true)
        v.position = make_float3(0.5f * width, -0.5f * height, 0);
    else
        v.position = make_float3(width, 0.0f, 0);
    
    v.normal = make_float3(0.0f, 0.0f, 1.0f);
    v.colour = colour;
    v.texCoord = make_float2(1.0f, 0);
    vertex_buffer_data.push_back(v);

    if (bCentralisedOrigin == true)
        v.position = make_float3(0.5f * width, 0.5f * height, 0);
    else
        v.position = make_float3(width, height, 0);
    
    v.normal = make_float3(0.0f, 0.0f, 1.0f);
    v.colour = colour;
    v.texCoord = make_float2(1.0f, 1.0f);
    vertex_buffer_data.push_back(v);

    if (bCentralisedOrigin == true)
        v.position = make_float3(-0.5f * width, 0.5f * height, 0);
    else
        v.position = make_float3(0.0f, height, 0);
    v.normal = make_float3(0.0f, 0.0f, 1.0f);
    v.colour = colour;
    v.texCoord = make_float2(0.f, 1.0f);
    
    vertex_buffer_data.push_back(v);

    index_buffer_data.push_back(3);
    index_buffer_data.push_back(0);
    index_buffer_data.push_back(2);
    index_buffer_data.push_back(1);
    index_buffer_data.push_back(2);
    index_buffer_data.push_back(0);
    
    CMesh* mesh = new CMesh(metalDevice);
    
    mesh->indexBuffer = metalDevice->newBuffer(index_buffer_data.size(), MTL::ResourceStorageModeShared);
    
    mesh->indexSize = index_buffer_data.size();
    mesh->mode = CMesh::DRAW_TRIANGLES;
    
    return mesh;
}
/**
 @brief Generate a quad at a specific position and load it into Metal
 @param colour A simd::float4 variable containing the colour of this quad
 @param x A const float variable of the x-coordinate of the quad
 @param y A const float variable of the y-coordinate of the quad
 @param width A const float variable containing the width of the quad
 @param height A const float variable containing the height of the quad
 @param metalDevice A metal compatible device (Either mac or iOS)
 */
CMesh* CMeshBuilder::GenerateQuadWithPosition(  const simd::float4 colour,
                                                const float x,
                                                const float y,
                                                const float width,
                                                const float height,
                                                MTL::Device* metalDevice)
{
    CMesh* mesh = new CMesh(metalDevice);
    
    return mesh;
}
