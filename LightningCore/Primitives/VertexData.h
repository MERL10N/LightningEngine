//
//  VertexData.h
//  LightningCore
//
//  Created by Kian Marvi on 3/13/25.
//

#ifndef VertexData_h
#define VertexData_h

#include <simd/simd.h>

using namespace simd;

struct VertexData
{
    float4 position;
    float4 normal;
    float2 textureCoordinate;
};

struct TransformationData
{
    float4x4 modelMatrix;
    float4x4 viewMatrix;
    float4x4 perspectiveMatrix;
};

#endif /* VertexData_h */
