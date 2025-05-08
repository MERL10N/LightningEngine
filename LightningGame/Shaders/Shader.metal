//
//  Shader.metal
//  LightningGame
//  Based on GLSL code from LearnOpenGL
//  Created by Kian Marvi on 5/7/25.

#include <metal_stdlib>
using namespace metal;

// Vertex input with position and color
struct VertexIn
{
    float3 aPos [[attribute(0)]];
    float3 aColor [[attribute(1)]];
};

// Output from vertex to fragment
struct VertexOut
{
    float4 position [[position]];
    float3 ourColor;
};

// Vertex shader
vertex VertexOut vertexShader(VertexIn in [[stage_in]])
{
    VertexOut out;
    out.position = float4(in.aPos, 1.0);
    out.ourColor = in.aColor;
    return out;
}

// Fragment shader
fragment float4 fragmentShader(VertexOut out [[stage_in]])
{
    return float4(out.ourColor, 1.0);
}
