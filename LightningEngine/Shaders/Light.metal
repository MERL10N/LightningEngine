//
//  Light.metal
//  LightningEngine - macOS
//
//  Created by Kian Marvi on 9/13/24.
//

#include <metal_stdlib>
using namespace metal;

struct VertexData
{
    float4 position [[position]];
    float4 normal;
};

vertex VertexData vertexShader(uint vertexID [[vertex_id]],
             constant VertexData* vertexData,
             constant float4x4& modelMatrix [[buffer(1)]],
             constant float4x4& viewMatrix [[buffer(2)]],
             constant float4x4& perspectiveMatrix [[buffer(3)]])
{
    VertexData out = vertexData[vertexID];
    
    out.position = perspectiveMatrix * viewMatrix * modelMatrix * vertexData[vertexID].position;
    return out;
}

fragment float4 fragmentShader(VertexData in [[stage_in]],
                                    constant float4& lightColor [[ buffer(0) ]]) 
{
    return lightColor;
}
