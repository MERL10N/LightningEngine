//  Skybox.metal
//  LightningEditor
//  Created by Kian Marvi on 8/12/26.
//  Credit: https://metalbyexample.com/reflection-and-refraction/
//  Metal By Example article was very helpful in providing me understanding in implementing skybox

#include <metal_stdlib>
using namespace metal;

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
    float4 texCoords;
};

struct Uniforms
{
    float4x4 projection;
    float4x4 view;
    float4x4 model;
};

struct CubeMapArguments
{
    texturecube<float> cubemap;
};

vertex VertexOut vertex_skybox(constant VertexIn* in [[buffer(0)]],
                             constant Uniforms &uniforms[[buffer(1)]],
                             uint vertexID [[vertex_id]],
                             uint instanceID [[instance_id]])
{
    
    float4 position = float4(in[vertexID].aPosition, 1.0f);
    
    VertexOut out;
    out.position = uniforms.projection * uniforms.view * uniforms.model * position;
    out.texCoords = position;
    
    return out;
}

fragment float4 fragment_skybox(VertexOut out [[stage_in]],
                                constant CubeMapArguments& cubeMapArgs[[buffer(0)]],
                                constant Uniforms &uniforms[[buffer(1)]])
{
    constexpr sampler textureSampler (mag_filter::linear, min_filter::linear);
    
    float3 texCoords = float3(out.texCoords.x, out.texCoords.y, -out.texCoords.z);
    
    return cubeMapArgs.cubemap.sample(textureSampler, texCoords);
}

