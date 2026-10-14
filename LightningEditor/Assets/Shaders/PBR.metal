//
//  PBR.metal (WIP)
//  LightningEditor
//
//  Created by Kian Marvi on 8/25/26.
//  Based on LearnOpenGL article on PBR: https://learnopengl.com/PBR/Lighting

#include <metal_stdlib>
using namespace metal;

struct VertexIn
{
    float3 aPosition;
    float3 aColor;
    float3 aNormal;
    float2 aTexCoord;
    float3 aTangent;
    float3 aBitangent;
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
};

struct InstancedUniforms
{
    float4x4 model;
};


struct Material
{
    texture2d<float> textureMaps[[id(0)]] [5];
};

float DistributionGGX(float3 N, float3 H, float roughness)
{
    float a = roughness * roughness;
    float a2 = a * a;
    float NDotH = max(dot(N,H), 0.0f);
    float NDotH2 = NDotH * NDotH;
    
    float num   = a2;
    float denom = (NDotH2 * (a2 - 1.0) + 1.0);
    denom = 3.14159265359f * denom * denom;
    
    return num / denom;
}

float GeometrySchlickGGX(float NDotV, float roughness)
{
    float r = (roughness + 1.0);
    float k = (r*r) / 8.0;

    float num   = NDotV;
    float denom = NDotV * (1.0 - k) + k;
    
    return num / denom;
}

float GeometrySmith(float3 N, float3 V, float3 L, float roughness)
{
    float NdotV = max(dot(N, V), 0.0);
    float NdotL = max(dot(N, L), 0.0);
    float ggx2  = GeometrySchlickGGX(NdotV, roughness);
    float ggx1  = GeometrySchlickGGX(NdotL, roughness);

    return ggx1 * ggx2;
}

float3 fresnelSchlick(float cosTheta, float3 F0)
{
    return F0 + (1.0 - F0) * pow(clamp(1.0 - cosTheta, 0.0, 1.0), 5.0);
}

// Vertex shader
vertex VertexOut vertex_main(constant VertexIn* in[[buffer(0)]],
                             constant Uniforms &uniforms[[buffer(1)]],
                             constant LightUniforms &lightUniforms[[buffer(2)]],
                             constant InstancedUniforms* instancedUniforms [[buffer(3)]],
                             uint vertexID   [[vertex_id]],
                             uint instanceID [[instance_id]])
{
    VertexOut out;
    out.texCoords = in.aTexCoords;
    out.position  = in.aPosition;
    return out;
}

// Fragment shader
fragment float4 fragment_main(VertexOut out [[stage_in]],
                             constant Material& material[[buffer(0)]],
                             constant LightUniforms &lightUniforms[[buffer(1)]])
{
    constexpr sampler textureSampler (mag_filter::linear, min_filter::linear, mip_filter::linear, max_anisotropy(16), address::clamp_to_edge);
    
    // Set the default values
    float4 albedoMap    = float4(1.0f);
    float4 normalMap    = float4(1.0f);
    float4 metallicMap  = float4(1.0f);
    float4 roughnessMap = float4(1.0f);
    float4 aoMap        = float4(1.0f);
    
    float3 normal       = float3(0.0f, 0.0f, 1.0f);
    float  metallic     = 1.0f;
    float  roughness    = 1.0f;
    float  ao           = 1.0f;
    
    // Check if the texture maps are nullptr
    if (!is_null_texture(material.textureMaps[0]))
    {
        albedoMap  = material.albedoMap.sample(textureSampler, out.texCoord);
    }
    
    if (!is_null_texture(material.textureMaps[1]))
    {
        normalMap   = material.normalMap.sample(textureSampler, out.texCoord);
        
        // Transform normal vector to range [-1, 1]
        normal = normalize(normalMap.xyz * 2.0f - 1.0f); // This normal is in tangent space
        
        float3 Q1 = ddx(out.position);
        float3 Q2 = ddy(out.position);
        float2 st1 = ddx(out.texCoords);
        float2 st2 = ddy(out.texCoords);
        
        float3 N = normal;
        float3 T = normalize(Q1 * st2.t - Q2 * st1.t);
        float3 B = -normalize(cross(N,T));
        float3x3 TBN = float3x3(T, B, N);
        
        normal = normalize(TBN * normal);
    }
    
    if (!is_null_texture(material.metallicMap))
    {
        metallic = material.metallicMap.sample(textureSampler, out.texCoord).r;
    }
    
    if (!is_null_texture(material.roughnessMap))
    {
        roughness = material.roughnessMap.sample(textureSampler, out.texCoord).r;
    }
    
    if (!is_null_texture(material.aoMap))
    {
        ao = material.aoMap.sample(textureSampler, out.texCoord).r;
    }
    
    
    if (albedoMap.a < 0.1)
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
    
    float nDotH = max(dot(normal, halfwayDir), 0.0f);
    
    float3 specular = specularStrength * spec * lightUniforms.lightColor * nDotH * specularMap.xyz;

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

