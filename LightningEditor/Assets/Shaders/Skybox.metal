//
//  Skybox.metal
//  LightningEditor
//
//  Created by Kian Marvi on 8/12/26.
//

#include <metal_stdlib>
using namespace metal;

struct VertexIn
{
    float3 aPosition  [[attribute(0)]];
    float2 aTexCoord  [[attribute(1)]];
};

struct VertexOut
{
    float3 position;
    float2 texCoord;
};

struct Uniforms
{
    float4x4 perspective;
    float4x4 view;
};
/*
vertex VertexOut vertex_main(VertexIn in [[stage_in]],
                             constant Uniforms &uniforms[[buffer(1)]],
                             uint instanceID [[instance_id]])
{
    VertexOut out;
    out.position = in.aPosition;
    out.texCoord = in.aTexCoord;
    return out;
}
*/
