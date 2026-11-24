//  Light.metal
//  LightningGame
//  Based on GLSL code from LearnOpenGL
//  Created by Kian Marvi on 3/30/26.

#include <metal_stdlib>
using namespace metal;

struct VertexIn
{
    float3 aPosition;
    float3 aColor;
    float3 aNormal;
    float2 aTexCoord;
    float3 aTangent;
    float3 aBitangent;
};

struct VertexOut
{
    float4 position [[position]];
};

struct Uniforms
{
    float4x4 perspective;
    float4x4 view;
};

// Prepare for instanced rendering
struct InstancedUniforms
{
    float4x4 model;
};

vertex VertexOut vertex_light(device const VertexIn* in [[buffer(0)]],
                              constant Uniforms &uniforms[[buffer(1)]],
                              device const InstancedUniforms* instancedUniforms[[buffer(3)]],
                              uint vertexID [[vertex_id]],
                              uint instanceID [[instance_id]])
{
    VertexOut out;
    float3 pos = in[vertexID].aPosition;
    out.position = float4(uniforms.perspective * uniforms.view * instancedUniforms[instanceID].model * float4(pos, 1.0f));
    return out;
}

fragment float4 fragment_light(VertexOut out [[stage_in]],
                               constant float3& lightColor [[buffer(0)]])
{
    return float4(lightColor, 1.0f);
}
