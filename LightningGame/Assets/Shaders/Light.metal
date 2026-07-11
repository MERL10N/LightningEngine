//  Light.metal
//  LightningGame
//  Based on GLSL code from LearnOpenGL
//  Created by Kian Marvi on 3/30/26.

#include <metal_stdlib>
using namespace metal;

struct VertexIn
{
    float3 position  [[attribute(0)]];
    float3 color     [[attribute(1)]];
    float3 normal    [[attribute(2)]];
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
vertex VertexOut vertex_light(VertexIn in [[stage_in]],
                                   constant Uniforms &uniform [[buffer(0)]])
{
    VertexOut out;
    float3 pos = in.position;
    out.position = float4(uniform.perspective * uniform.view * uniform.model * float4(pos, 1.0f));
    return out;
}

fragment float4 fragment_light(VertexOut out [[stage_in]],
                               constant float3& lightColor [[buffer(0)]])
{
    return float4(lightColor, 1.0f);
}
