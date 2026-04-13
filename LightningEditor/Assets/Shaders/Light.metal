//  Light.metal
//  LightningGame
//  Based on GLSL code from LearnOpenGL
//  Created by Kian Marvi on 3/30/26.

#include <metal_stdlib>
using namespace metal;

struct VertexIn
{
    float3 position [[attribute(0)]];
    float3 color     [[attribute(1)]];
    float3 normal [[attribute(2)]];
};

struct VertexOut
{
    float4 position [[position]];
    float3 normal;
    half3 color;
};

vertex VertexOut vertex_light(VertexIn in [[stage_in]],
                                   constant float4x4 &transform[[buffer(1)]],
                                   constant float4x4 &projection[[buffer(2)]],
                                   constant float4x4 &view[[buffer(3)]])
{
    VertexOut out;
    half3 pos = half3(in.position);
    out.position = float4(half4x4(projection) * half4x4(view) * half4x4(transform) * half4(pos, 1.0f));
    out.color = half3(in.color);
    return out;
}

fragment half4 fragment_light(VertexOut out [[stage_in]])
{
    return half4(out.color, 1.0f);
}
