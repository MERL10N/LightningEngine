//
//  Shader.metal
//  LightningGame
//  Based on GLSL code from LearnOpenGL and MetalTutorial.com
//  Created by Kian Marvi on 5/7/25.

#include <metal_stdlib>
using namespace metal;

// Vertex input with position and color
struct VertexIn
{
    float3 aPos [[attribute(0)]];
    float3 aColor [[attribute(1)]];
    float2 aTexCoord[[attribute(2)]];
};

// Output from vertex to fragment
struct VertexOut
{
    float4 position [[position]];
    float3 ourColor;
    float2 TexCoord;
};

// Vertex shader
vertex VertexOut vertexShader(VertexIn in [[stage_in]])
{
    VertexOut out;
    out.position = float4(in.aPos, 1.0);
    out.ourColor = in.aColor;
    out.TexCoord = in.aTexCoord;
    return out;
}

// Fragment shader
fragment float4 fragmentShader(VertexOut out [[stage_in]],
                               texture2d<float> colorTexture [[texture(0)]])
{
    constexpr sampler textureSampler (mag_filter::linear,
                                      min_filter::linear);
    // Sample the texture to obtain a color
    const float4 colorSample = colorTexture.sample(textureSampler, out.TexCoord);
    return colorSample;
}
