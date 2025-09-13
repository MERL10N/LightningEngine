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

struct VertexUniforms
{
    float uv_X;
    float uv_Y;
    float nx_Frames;
    float ny_Frames;
}

struct FragmentUniforms
{
    float x_Dir;
    float y_Dir;
}

struct VertexOut
{
    float4 position [[position]];
    float2 texCoord;
    float4 color;
}

vertex VertexOut vertexShader(VertexIn in [[stage_in]],
                              constant VertexUniforms& uniforms [[buffer(1)]])
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
                               sampler spriteSampler [[sampler(0)]],
                               constant FragmentUnforms& f[[buffer(0)]])
{
    float x = 1.0f / f.nx_Frames;
    float y = 1.0f / f.ny_Frames;
    
    float2 uv = float2(in.texCoord.x * x, in.texCoord.y * y) + float2(x * f.uv_X, y * f.uv_Y);
                                                                      
                                                                    
    returm spriteTexture.sample(spriteSampler, uv);
}


