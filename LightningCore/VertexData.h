//
//  VertexData.h
//  LightningCore
//
//  Created by Kian Marvi on 12/12/25.
//
#ifndef VERTEX_DATA_H
#define VERTEX_DATA_H
#include <simd/simd.h>

using namespace simd;

struct VertexData
{
    float4 position;
    float2 textureCoordinate;
};

struct TransformationData
{
    float4x4 modelMatrix;
    float4x4 viewMatrix;
    float4x4 perspectiveMatrix;
};
#endif
