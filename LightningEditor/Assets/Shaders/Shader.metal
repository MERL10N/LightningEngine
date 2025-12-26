//  Shader.metal
//  LightningGame
//  Based on GLSL code from LearnOpenGL and GetIntoGameDev tutorials
//  Created by Kian Marvi on 5/7/25.

#include <metal_stdlib>
using namespace metal;

struct VertexIn
{
    float3 position [[attribute(0)]];
    float3 color [[attribute(1)]];
    float2 texCoord[[attribute(2)]];
};

struct VertexOut
{
    float4 position [[position]];
    half3 color;
    float2 texCoord;
};

// Vertex shader
vertex VertexOut vertexShader(VertexIn in [[stage_in]], constant float4x4 &transform[[buffer(1)]], constant float4x4 &projection[[buffer(2)]], constant float4x4 &view[[buffer(3)]])
{
    VertexOut out;
    half3 pos = half3(in.position);
    out.position = float4(half4x4(projection) * half4x4(transform) * half4(pos, 1.0f));
    out.color = half3(in.color);
    out.texCoord = in.texCoord;
    return out;
}

// Fragment shader
fragment half4 fragmentShader(VertexOut out [[stage_in]],
                               texture2d<half> colorTexture [[texture(0)]])
{
    constexpr sampler textureSampler (mag_filter::linear, min_filter::linear);
    // Sample the texture to obtain a color
    half4 colorSample = colorTexture.sample(textureSampler, out.texCoord);
    return colorSample * half4(out.color, 1.0f);
}
