//  Light.metal
//  LightningGame
//  Based on GLSL code from LearnOpenGL
//  Created by Kian Marvi on 3/30/26.

#include <metal_stdlib>
using namespace metal;

struct VertexOut
{
    float4 position [[position]];
    float4 normal;
    
};

