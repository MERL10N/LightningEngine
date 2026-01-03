//
//  SubTexture.cpp
//  LightningCore
//
//  Created by Kian Marvi on 12/30/25.
//

#include <stdio.h>
#include "Metal/Metal.hpp"
#include "SubTexture.h"


SubTexture::SubTexture(MTL::Texture *texture, const simd::float2 &min, const simd::float2 &max)
: m_Texture(texture)
{
    m_TexCoords[0] = simd_make_float2(min.x, max.y);
    m_TexCoords[1] = simd_make_float2(max.x, max.y);
    m_TexCoords[2] = simd_make_float2(max.x, min.y);
    m_TexCoords[3] = simd_make_float2(min.x, min.y);
}

SubTexture SubTexture::CreateFromCoords(MTL::Texture *texture, const simd::float2 &coords, const simd::float2 &spriteSize)
{
    simd::float2 min = {(coords.x * spriteSize.x) / texture->width(),  (coords.y * spriteSize.y)/ texture->height() };
    simd::float2 max = {(coords.x + 1) * spriteSize.x / texture->width(),  (coords.y + 1) * spriteSize.y/ texture->height() };
    return SubTexture(texture, min, max);
}

