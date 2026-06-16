//
//  ShaderUniforms.h
//  LightningCore
//
//  Created by Kian Marvi on 4/14/26.
//

#ifndef ShaderUniforms_h
#define ShaderUniforms_h

#include <simd/simd.h>
struct LightData
{
    simd::float3 m_LightColor;
    simd::float3 m_LightPosition;
};

#endif /* ShaderUniforms_h */
