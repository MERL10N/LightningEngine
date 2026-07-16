//
//  ShaderUniforms.h
//  LightningCore
//
//  Created by Kian Marvi on 4/14/26.
//

#ifndef ShaderUniforms_h
#define ShaderUniforms_h

#define HLSLPP_FEATURE_TRANSFORM
#include <hlsl++.h>

struct Uniforms
{
    float4x4 projection;
    float4x4 view;
    float4x4 model;
};
struct LightUniforms
{
    float3 m_LightColor;
    float3 m_LightPosition;
    float3 m_CameraPosition;
};

struct InstancedUniforms
{
    float4x4 model;
};

#endif /* ShaderUniforms_h */
