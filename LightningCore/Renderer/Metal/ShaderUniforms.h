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

class MetalTexture;

struct Uniforms
{
    float4x4 projection;
    float4x4 view;
    float4x4 model;
};
struct LightUniforms
{
    float3 lightColor;
    float3 lightPosition;
    float3 cameraPosition;
};

struct InstancedUniforms
{
    float4x4 model;
};

#endif /* ShaderUniforms_h */
