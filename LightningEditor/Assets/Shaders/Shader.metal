//  Shader.metal
//  LightningGame
//  Based on GLSL code from LearnOpenGL and GetIntoGameDev tutorials
//  Created by Kian Marvi on 5/7/25.

#include <metal_stdlib>
using namespace metal;

struct Light
{
    float3 direction;
    
    float3 ambient;
    float3 diffuse;
    float3 specular;
    
    float constantValue;
    float linear;
    float quadratic;
    
};

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
    
    float3 worldPosition;
    float3 color;
    float3 normal;
    float2 texCoord;
    Light light;
};

struct ArgumentBufferTexture
{
    texture2d<float> colorTexture[[id(0)]];
};

// Vertex shader
vertex VertexOut vertex_main(VertexIn in [[stage_in]],
                             constant float4x4 &perspective[[buffer(1)]],
                             constant float4x4 &view[[buffer(2)]],
                             constant float4x4 &model[[buffer(3)]])
{
    VertexOut out;
    float4 worldPos = model * float4(in.position, 1.0f);
    out.worldPosition = worldPos.xyz;
    out.position = perspective * view * worldPos;
    out.normal = in.normal;
    out.color =  in.color;
    out.texCoord = in.texCoord;
    return out;
}

// Fragment shader
fragment float4 fragment_main(VertexOut out [[stage_in]],
                             constant ArgumentBufferTexture &args[[buffer(0)]],
                             constant float3& lightColor[[buffer(1)]],
                             constant float3& lightPosition[[buffer(2)]],
                             constant float3& cameraPosition[[buffer(3)]])
{
    // Ambient
    float ambientStrength = 0.1f;
    float3 ambient = ambientStrength * lightColor;
        
    // Diffuse
    float3 norm = normalize(out.normal.xyz);
    float3 lightDir = normalize(lightPosition - float3(out.worldPosition));
    float diff = max(dot(norm, lightDir.xyz), 0.0);
    float3 diffuse = diff * lightColor;
        
    // Specular
    float specularStrength = 1.0f;
    float3 viewDir = normalize(cameraPosition - float3(out.worldPosition));
    float3 halfwayDir = normalize(lightDir + viewDir);
    float spec = pow(max(dot(norm, halfwayDir), 0.0f), 32);
        
    float3 specular = specularStrength * spec * lightColor;
    constexpr sampler textureSampler (mag_filter::linear, min_filter::linear);
    
    float3 result = (ambient + diffuse + specular) * out.color;
    // Sample the texture to obtain a color
    float4 colorSample = args.colorTexture.sample(textureSampler, out.texCoord);
    
    if (colorSample.a < 0.1)
    {
        discard_fragment();
    }
    
    return colorSample * float4(result, 1.0f);
}

// Fragment shader
fragment float4 fragment_main_untextured(VertexOut out [[stage_in]],
                                        constant float3& lightColor[[buffer(0)]],
                                        constant float3& lightPosition[[buffer(1)]],
                                        constant float3& cameraPosition[[buffer(2)]])
{
    // Ambient
    float ambientStrength = 0.1f;
    float3 ambient = ambientStrength * lightColor;
        
    // Diffuse
    float3 norm = normalize(out.normal.xyz);
    float3 lightDir = normalize(lightPosition - float3(out.worldPosition));
    float diff = max(dot(norm, lightDir.xyz), 0.0);
    float3 diffuse = diff * lightColor;
        
    // Specular
    float specularStrength = 1.0f;
    float3 viewDir = normalize(cameraPosition - float3(out.worldPosition));
    float3 halfwayDir = normalize(lightDir + viewDir);
    float spec = pow(max(dot(norm, halfwayDir), 0.0f), 32);
        
    float3 specular = specularStrength * spec * lightColor;
    float3 finalColor = (ambient + diffuse + specular) * out.color;
    
    return float4(finalColor, 1.0f);
}

