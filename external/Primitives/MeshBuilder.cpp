/*
    MeshBuilder.cpp
    LightningEngine

    Created by Kian Marvi on 2/2/24.
*/

#include "MeshBuilder.hpp"

/**
 @brief Generate a quad and load it into Metal
 @param colour A simd::float4 variable containing the colour of this quad
 @param width A const float variable containing the width of the quad
 @param height A const float variable containing the height of the quad
 @param bCentralisedOrigin A bool variable to decide if the quad is centralised around the origin
 */

CMesh* CMeshBuilder::GenerateQuad(simd::float4 colour, const float width, const float height, bool bCentralisedOrigin)
{
}

/**
 @brief Generate a quad at a specific position and load it into Metal
 @param colour A simd::float4 variable containing the colour of this quad
 @param x A const float variable of the x-coordinate of the quad
 @param y A const float variable of the y-coordinate of the quad
 @param width A const float variable containing the width of the quad
 @param height A const float variable containing the height of the quad
 */
CMesh* CMeshBuilder::GenerateQuadWithPosition(  const simd::float4 colour,
                                                const float x,
                                                const float y,
                                                const float width,
                                                const float height)
{
}
