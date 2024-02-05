/*
    MeshBuilder.cpp
    LightningEngine

    Created by Kian Marvi on 2/2/24.
*/

#include "MeshBuilder.h"
#include "VertexData.hpp"

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
    CMesh* mesh = new CMesh(metalDevice);
    
    return mesh;
}

//void CMeshBuilder::GenerateQuad(MTL::Buffer* squareVertexBuffer, MTL::Device* metalDevice)
//{
//    VertexData squareVertices[]
//    {
//            {{-0.5, -0.5,  0.5, 1.0f}, {0.0f, 0.0f}},
//            {{-0.5,  0.5,  0.5, 1.0f}, {0.0f, 1.0f}},
//            {{ 0.5,  0.5,  0.5, 1.0f}, {1.0f, 1.0f}},
//            {{-0.5, -0.5,  0.5, 1.0f}, {0.0f, 0.0f}},
//            {{ 0.5,  0.5,  0.5, 1.0f}, {1.0f, 1.0f}},
//            {{ 0.5, -0.5,  0.5, 1.0f}, {1.0f, 0.0f}}
//    };
//
//    squareVertexBuffer = metalDevice->newBuffer(&squareVertices, sizeof(squareVertices), MTL::ResourceStorageModeShared);
//    CImageLoader::GetInstance()->LoadTexture("assets/mc_grass.png", metalDevice);
//}

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
