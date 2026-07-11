//
//  ShaderUniforms.h
//  LightningCore
//
//  Created by Kian Marvi on 4/14/26.
//

#ifndef ShaderUniforms_h
#define ShaderUniforms_h

#include <simd/simd.h>

struct Uniforms
{
    simd::float4x4 perspective;
    simd::float4x4 view;
    simd::float4x4 model;
};
struct LightUniforms
{
    simd::float3 m_LightColor;
    simd::float3 m_LightPosition;
    simd::float3 m_CameraPosition;
};

#endif /* ShaderUniforms_h */
