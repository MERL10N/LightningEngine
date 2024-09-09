//
//  Cube.metal
//  LightningEngine
//
//  Created by Kian Marvi on 7/19/24.
//

#include <metal_stdlib>
using namespace metal;

struct VertexData
{
    float4 position[[id(0)]];
    float2 textureCoordinate[[id(1)]];;
};

struct TransformationData
{
    float4x4 modelMatrix[[id(0)]];
    float4x4 viewMatrix[[id(1)]];
    float4x4 perspectiveMatrix[[id(2)]];
};

struct VertexOut
{
    float4 position [[position]];

    float2 textureCoordinate;
};

vertex VertexOut vertexShader(uint vertexID [[vertex_id]],
             constant VertexData* vertexData,
             constant TransformationData* transformationData)
{
    VertexOut out;
    out.position = transformationData->perspectiveMatrix * transformationData->viewMatrix * transformationData->modelMatrix * vertexData[vertexID].position;
    out.textureCoordinate = vertexData[vertexID].textureCoordinate;
    return out;
}

fragment float4 fragmentShader(VertexOut in [[stage_in]],
                               texture2d<float> colorTexture [[texture(0)]])
{
    constexpr sampler textureSampler (mag_filter::linear,
                                      min_filter::linear);
    
    // Sample the texture to obtain a color
    const float4 colorSample = colorTexture.sample(textureSampler, in.textureCoordinate);
    return colorSample;
}
