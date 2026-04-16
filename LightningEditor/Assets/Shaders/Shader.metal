//  Shader.metal
//  LightningGame
//  Based on GLSL code from LearnOpenGL and GetIntoGameDev tutorials
//  Created by Kian Marvi on 5/7/25.

#include <metal_stdlib>
using namespace metal;

struct VertexIn
{
    float3 position  [[attribute(0)]];
    float3 color     [[attribute(1)]];
    float3 normal    [[attribute(2)]];
    float2 texCoord  [[attribute(3)]];
};

struct VertexOut
{
    float4 position [[position]];
    half3 color;
    float3 normal;
    float2 texCoord;
};

struct ArgumentBufferTexture
{
    texture2d<half> colorTexture[[id(0)]];
};

// Vertex shader
vertex VertexOut vertex_main(VertexIn in [[stage_in]],
                             constant float4x4 &transform[[buffer(1)]],
                             constant float4x4 &projection[[buffer(2)]],
                             constant float4x4 &view[[buffer(3)]])
{
    VertexOut out;
    half3 pos = half3(in.position);
    out.position = float4(half4x4(projection) * half4x4(view) * half4x4(transform) * half4(pos, 1.0f));
    out.normal = in.normal;
    out.color = half3(in.color);
    out.texCoord = in.texCoord;
    return out;
}

// Fragment shader
fragment half4 fragment_main(VertexOut out [[stage_in]],
                             constant ArgumentBufferTexture &args[[buffer(0)]],
                             constant float3& lightColor[[buffer(1)]],
                             constant float3& lightPosition[[buffer(2)]],
                             constant float3& cameraPosition[[buffer(3)]])
{
    // Ambient
    float ambientStrength = 0.2f;
    float3 ambient = ambientStrength * lightColor;
        
    // Diffuse
    float3 norm = normalize(out.normal.xyz);
    float3 lightDir = normalize(lightPosition - float3(out.position));
    float diff = max(dot(norm, lightDir.xyz), 0.0);
    float3 diffuse = diff * lightColor;
        
    // Specular
    float specularStrength = 1.0f;
    float3 viewDir = normalize(cameraPosition - float3(out.position));
    float3 halfwayDir = normalize(lightDir + viewDir);
    float spec = pow(max(dot(norm, halfwayDir), 0.0), 32);
        
    float3 specular = specularStrength * spec * lightColor;
    constexpr sampler textureSampler (mag_filter::linear, min_filter::linear);
    
    half3 finalColor = half3(ambient + diffuse + specular) * out.color;
    // Sample the texture to obtain a color
    half4 colorSample = args.colorTexture.sample(textureSampler, out.texCoord);
    
    if (colorSample.a < 0.1)
    {
        discard_fragment();
    }
    
    return colorSample * half4(finalColor, 1.0f);
}

// Fragment shader
fragment half4 fragment_main_untextured(VertexOut out [[stage_in]])
{
    return half4(out.color, 1.0f);
}

