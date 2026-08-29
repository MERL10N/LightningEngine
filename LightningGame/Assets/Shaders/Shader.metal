//  Shader.metal
//  LightningGame
//  Based on GLSL code from LearnOpenGL and GetIntoGameDev tutorials
//  Created by Kian Marvi on 5/7/25.

#include <metal_stdlib>
using namespace metal;
/*
 
 TODO:
 - [X] Implement Lightmaps
 - [X] Implement Normal Mapping
 - [] Implement Instanced Rendering
 - [] Implement Shadow Mapping
 - [] Implement Deferred Rendering
*/

struct Light
{
    float3 direction;
    float3 ambient;
    float3 diffuse;
    float3 specular;
    
};

struct VertexIn
{
    float3 aPosition  [[attribute(0)]];
    float3 aColor     [[attribute(1)]];
    float3 aNormal    [[attribute(2)]];
    float2 aTexCoord  [[attribute(3)]];
    float3 aTangent   [[attribute(4)]];
    float3 aBitangent [[attribute(5)]];
};

struct VertexOut
{
    float4 position [[position]];
    float3 fragmentPosition;
    float3 color;
    float3 normal;
    float2 texCoord;
    float3 T;
    float3 B;
    float3 N;
    float3 TangentLightPos;
    float3 TangentViewPos;
    float3 TangentFragPos;
};

struct LightUniforms
{
    float3 lightColor;
    float3 lightPosition;
    float3 cameraPosition;
};

struct Uniforms
{
    float4x4 perspective;
    float4x4 view;
    float4x4 model; // <- Soon will be removed
};


// Prepare for instanced rendering
struct InstancedUniforms
{
    float4x4 model;
};


struct Material
{
    texture2d<float> textureMaps[[id(0)]] [4];
};

// Vertex shader
vertex VertexOut vertex_main(VertexIn in [[stage_in]],
                             constant Uniforms &uniforms[[buffer(1)]],
                             constant LightUniforms &lightUniforms[[buffer(2)]],
                             uint instanceID [[instance_id]])
{
    VertexOut out;
    out.position         = uniforms.perspective * uniforms.view * uniforms.model * float4(in.aPosition, 1.0f);
    out.fragmentPosition = float3(uniforms.model * float4(in.aPosition, 1.0f));
    out.normal           = in.aNormal;
    out.color            = in.aColor;
    out.texCoord         = in.aTexCoord;
    
    
    out.T               = normalize(float3(uniforms.model * float4(in.aTangent, 0.0f)));
    out.N               = normalize(float3(uniforms.model * float4(in.aNormal, 0.0f)));
    out.T               = normalize(out.T - dot(out.T,out.N) * out.N);
    out.B               = cross(out.N, out.T);
    
    float3x3 TBN = transpose(float3x3(out.T, out.B, out.N));
    out.TangentLightPos = TBN * lightUniforms.lightPosition;
    out.TangentViewPos  = TBN * lightUniforms.cameraPosition;
    out.TangentFragPos  = TBN * out.fragmentPosition;
    
    return out;
}

// Fragment shader
fragment float4 fragment_main(VertexOut out [[stage_in]],
                             constant Material& textureArgs[[buffer(0)]],
                             constant LightUniforms &lightUniforms[[buffer(1)]])
{
    constexpr sampler textureSampler (mag_filter::linear, min_filter::linear);
    
    // Set the default values
    float4 diffuseMap  = float4(1.0f);
    float4 specularMap = float4(1.0f);
    float4 normalMap   = float4(1.0f);
    float3 normal      = float3(0.0f, 0.0f, 1.0f);
    
    // Check if the texture maps are nullptr
    if (!is_null_texture(textureArgs.textureMaps[0]))
    {
        diffuseMap  = textureArgs.textureMaps[0].sample(textureSampler, out.texCoord);
    }
    
    if (!is_null_texture(textureArgs.textureMaps[1]))
    {
        specularMap = textureArgs.textureMaps[1].sample(textureSampler, out.texCoord);
    }
    
    if (!is_null_texture(textureArgs.textureMaps[2]))
    {
        normalMap   = textureArgs.textureMaps[2].sample(textureSampler, out.texCoord);
        
        // Transform normal vector to range [-1, 1]
        normal = normalize(normalMap.xyz * 2.0f - 1.0f); // This normal is in tangent space
    }
    
    if (diffuseMap.a < 0.1)
    {
        discard_fragment();
    }
    
    // Ambient
    float ambientStrength = 0.1f;
    float3 ambient = ambientStrength * lightUniforms.lightColor * diffuseMap.xyz;
 
    
    float3 lightDir = normalize(out.TangentLightPos - out.TangentFragPos);
    float diff = max(dot(lightDir.xyz, normal), 0.0);
    float3 diffuse = diff * lightUniforms.lightColor * diffuseMap.xyz;
        
    // Specular
    float specularStrength = 0.5f;
    
    float3 viewDir = normalize(out.TangentViewPos - float3(out.TangentFragPos));
    float3 halfwayDir = normalize(lightDir + viewDir);
    
    float specularTerm = max(dot(normal, halfwayDir), 0.0f);
    
    float spec = pow(max(specularTerm, 0.0f), 64.0f);
    
    float nDotL = max(dot(normal, halfwayDir), 0.0f);
    
    float3 specular = specularStrength * spec * lightUniforms.lightColor * nDotL * specularMap.xyz;

    float3 result = (ambient + diffuse + specular) * out.color;
    
    return float4(result, 1.0f);
}

// Fragment shader
fragment float4 fragment_main_untextured(VertexOut out [[stage_in]],
                                         constant LightUniforms &lightUniforms[[buffer(0)]])
{
    // Ambient
    float ambientStrength = 0.1f;
    float3 ambient = ambientStrength * lightUniforms.lightColor;
        
    // Diffuse
    float3 norm = normalize(out.normal.xyz);
    float3 lightDir = normalize(lightUniforms.lightPosition - float3(out.fragmentPosition));
    float diff = max(dot(lightDir.xyz, norm), 0.0);
    float3 diffuse = diff * lightUniforms.lightColor;
        
    // Specular
    float specularStrength = 1.0f;
    float3 viewDir = normalize(lightUniforms.cameraPosition - float3(out.fragmentPosition));
    float3 halfwayDir = normalize(lightDir + viewDir);
    float spec = pow(max(dot(norm, halfwayDir), 0.0f), 32);
        
    float3 specular = specularStrength * spec * lightUniforms.lightColor;
    float3 finalColor = (ambient + diffuse + specular) * out.color;
    
    return float4(finalColor, 1.0f);
}

