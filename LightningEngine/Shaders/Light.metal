//
//  Light.metal
//  LightningEngine - macOS
//
//  Created by Kian Marvi on 9/13/24.
//

#include <metal_stdlib>
using namespace metal;

struct LightVertexData
{
    float4 position [[position]];
    float4 normal;
};

struct TransformationData
{
    float4x4 modelMatrix;
    float4x4 viewMatrix;
    float4x4 perspectiveMatrix;
};


vertex LightVertexData vertexShader(uint vertexID[[vertex_id]],
                                    constant LightVertexData* vertexData,
                                    constant TransformationData* transformationData)
{
    LightVertexData out = vertexData[vertexID];
    
    out.position = transformationData->perspectiveMatrix * transformationData->viewMatrix * transformationData->modelMatrix * vertexData[vertexID].position;
    
    return out;
}

fragment float4 fragmentShader(LightVertexData in [[stage_in]], constant float4& lightColor [[buffer(0)]])
{
    return lightColor;
}
