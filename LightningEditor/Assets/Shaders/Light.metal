//  Light.metal
//  LightningGame
//  Based on GLSL code from LearnOpenGL
//  Created by Kian Marvi on 3/30/26.

#include <metal_stdlib>
using namespace metal;

struct VertexIn
{
    float3 aPosition  [[attribute(0)]];
    float3 aColor     [[attribute(1)]];
    float3 aNormal    [[attribute(2)]];
    float2 aTexCoord  [[attribute(3)]];
    float3 aTangent   [[attribute(4)]];
    float3 aBitangent [[attribute(5)]];
};

struct VertexOut
{
    float4 position [[position]];
    float3 normal;
    float3 color;
};

struct Uniforms
{
    float4x4 perspective;
    float4x4 view;
    float4x4 model;
};

// Prepare for instanced rendering
struct InstancedUniforms
{
    float4x4 model;
};

vertex VertexOut vertex_light(constant VertexIn* in [[buffer(0)]],
                              constant Uniforms &uniforms[[buffer(1)]],
                              uint vertexID [[vertex_id]],
                              uint instanceID [[instance_id]])
{
    VertexOut out;
    float3 pos = in[vertexID].aPosition;
    out.position = float4(uniforms.perspective * uniforms.view * uniforms.model * float4(pos, 1.0f));
    return out;
}

fragment float4 fragment_light(VertexOut out [[stage_in]],
                               constant float3& lightColor [[buffer(0)]])
{
    return float4(lightColor, 1.0f);
}
