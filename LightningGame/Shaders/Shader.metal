//
//  Shader.metal
//  LightningGame
//  Based on GLSL code from LearnOpenGL and MetalTutorial.com
//  Created by Kian Marvi on 5/7/25.

#include <metal_stdlib>
using namespace metal;

#pragma mark - Shader for standard 2D Render Pipeline
// Vertex input with position and color
struct VertexIn
{
    float3 aPos [[attribute(0)]];
    half3 aColor [[attribute(1)]];
    float2 aTexCoord[[attribute(2)]];
};

// Output from vertex to fragment
struct VertexOut
{
    float4 position [[position]];
    half3 ourColor;
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

#pragma mark - Shader for Sprite Animation

/*
  Sprite Animation Shader code based on OGLDEV tutorial video
 
  link: https://www.youtube.com/watch?v=BJq-SqMd9FM
 */

#define MAX_QUADS 100

struct VertexInput
{
    float2 Position [[attribute(0)]];
    unsigned int QuadID [[attribute(1)]];
};

struct QuadInfo
{
    float2 BasePos[MAX_QUADS];
    float2 WidthHeight[MAX_QUADS];
    float2 TexCoords[MAX_QUADS];
    float2 TexWidthHeight[MAX_QUADS];
};


struct VertexOutput
{
    float4 Position[[position]];
    float2 TexCoords;
};

vertex VertexOutput spriteVertexShader(VertexInput input [[stage_in]], constant QuadInfo& Quads [[buffer(1)]])
{
    VertexOutput out;
    
    float3 BasePosition = float3(Quads.BasePos[input.QuadID], 0.5);
    
    float2 WidthHeight = input.Position * Quads.WidthHeight[input.QuadID];
    
    float3 NewPosition = BasePosition + float3(WidthHeight, 0.0);
    
    out.Position = float4(NewPosition, 1.0);
    
    // Calculate tex-coords
    float2 BaseTexCoords = Quads.TexCoords[input.QuadID];
    
    float2 TexWidthHeight = input.Position * Quads.TexWidthHeight[input.QuadID];
    
    out.TexCoords = BaseTexCoords + TexWidthHeight;
    
    return out;
}

// Fragment shader
fragment float4 spriteFragmentShader(VertexOutput in [[stage_in]],
                               texture2d<float> colorTexture [[texture(0)]],
                               sampler s [[sampler(0)]])
{
    float4 FragColor = colorTexture.sample(s, in.TexCoords);
    
    if (all(FragColor == float4(0.0)))
    {
        discard_fragment();
    }
    
    return FragColor;
}
