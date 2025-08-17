//
//  Sprite.metal
//  LightningEditor
//
//  Created by Kian Marvi on 8/15/25.
//  Sprite Shader based on learnmetal.com and reference to author's molten engine

#include <metal_stdlib>
using namespace metal;

struct VertexIn
{
    float3 position [[attribute(0)]];
    float3 texCoord [[attribute(1)]];
    float3 color    [[attribute(2)]];
}

struct Uniforms
{
    float4x4 projectionMatrix;
}

struct VertexOut
{
    float4 position [[position]];
    float2 texCoord;
    float4 color;
}

vertex VertexOut vertexShader(VertexIn in [[stage_in]],
                              constant Uniforms& uniforms [[buffer(1)]])
{
    VertexOut out;
    float4 worldPos = float4(in.position, 1.0);
    out.position = uniforms.projectionMatrix * worldPos;
    out.texCoord = in.texCoord;
    out.color = in.color;
    return out;
}

fragment float4 fragmentShader(VertexOut in [[stage_in]],
                               texture2d<float> spriteTexture [[texture(0)]],
                               sampler spriteSampler [[sampler(0)]])
{
    if (spriteTexture.get_width() == 0) {
        // No texture bound, output vertex color only
        return in.color;
    }

    float4 texColor = spriteTexture.sample(spriteSampler, in.texCoord);

    // If texture sample is fully transparent (alpha == 0), fallback to vertex color
    if (texColor.a == 0.0) {
        return in.color;
    }

    return texColor * in.color;
}


