//
//  LightningMath.h
//  LightningCore
//
//  Created by Kian Marvi on 8/15/25.
//

#ifndef LightningMath_h
#define LightningMath_h

#ifdef __APPLE__
    #include <simd/simd.h>
typedef simd::float2 Vector2;
typedef simd::float3 Vector3;
typedef simd::float4 Vector4;
typedef simd::float2x2 Matrix2x2;
typedef simd::float3x3 Matrix3x3;
typedef simd::float4x4 Matrix4x4;
#endif

#endif /* LightningMath_h */
