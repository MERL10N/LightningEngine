#include <metal_stdlib>
using namespace metal;

vertex float4
vertexShader(uint vertexID [[vertex_id]],
             constant simd::float3* vertexPositions)
{
    float4 vertexOutPositions = float4(vertexPositions[vertexID][0],
                                       vertexPositions[vertexID][1],
                                       vertexPositions[vertexID][2],
                                       1.0f);
    return vertexOutPositions;
}

fragment float4 fragmentShader(float4 vertexOutPositions [[stage_in]])
{
    return float4(0.8f, 1.f, 1.f, 1.0f);
}
