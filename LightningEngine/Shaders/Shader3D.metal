//
//  Cube.metal
//  LightningEngine
//
//  Created by Kian Marvi on 7/19/24.
//

#include <metal_stdlib>
using namespace metal;

struct VertexData
{
    float4 position[[id(0)]];
    float4 normal[[id(1)]];
    float2 textureCoordinate[[id(2)]];;
};

struct TransformationData
{
    float4x4 modelMatrix[[id(0)]];
    float4x4 viewMatrix[[id(1)]];
    float4x4 perspectiveMatrix[[id(2)]];
};

struct VertexOut
{
    float4 position [[position]];
    float4 normal;
    float2 textureCoordinate;
    float4 fragmentPosition;
};

vertex VertexOut vertexShader(uint vertexID [[vertex_id]],
             constant VertexData* vertexData,
             constant TransformationData* transformationData)
{
    VertexOut out;
    out.position = transformationData->perspectiveMatrix * transformationData->viewMatrix * transformationData->modelMatrix * vertexData[vertexID].position;
    out.normal = vertexData[vertexID].normal;
    out.textureCoordinate = vertexData[vertexID].textureCoordinate;
    out.fragmentPosition = transformationData->modelMatrix * vertexData[vertexID].position;
    return out;
}

fragment float4 fragmentShader(VertexOut in [[stage_in]],
                               texture2d<float> colorTexture [[texture(0)]],
                               constant float4& lightColor     [[buffer(1)]],
                               constant float4& lightPosition  [[buffer(2)]],
                               constant float3& cameraPosition [[buffer(3)]])
{
    constexpr sampler textureSampler (mag_filter::linear,
                                      min_filter::linear);
    
    // Ambient
       float ambientStrength = 0.2f;
       float4 ambient = ambientStrength * lightColor;
       
       // Diffuse
       float3 norm = normalize(in.normal.xyz);
       float4 lightDir = normalize(lightPosition - in.fragmentPosition);
       float diff = max(dot(norm, lightDir.xyz), 0.0);
       float4 diffuse = diff * lightColor;
       
       // Specular
       float specularStrength = 0.5f;
       float4 viewDir = normalize(float4(cameraPosition, 1.0) - in.fragmentPosition);
       float4 reflectDir = reflect(-lightDir, float4(norm, 1));
       float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);
       float4 specular = specularStrength * spec * lightColor;
    
    // Sample the texture to obtain a color
    const float4 finalColor = (ambient + diffuse + specular) * colorTexture.sample(textureSampler, in.textureCoordinate);
    return finalColor;
}
