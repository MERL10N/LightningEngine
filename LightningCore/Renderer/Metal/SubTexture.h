//
//  SubTexture.h
//  LightningCore
//
//  Created by Kian Marvi on 12/30/25.
//

#ifndef SubTexture_h
#define SubTexture_h

namespace MTL
{
    class Texture;
}
#include <simd/simd.h>
class SubTexture
{
public:
    SubTexture(MTL::Texture* texture, const simd::float2 &min, const simd::float2 &max);
    inline const MTL::Texture* GetTexture() const { return m_Texture; }
    inline const simd::float2* GetTexCoords() const { return m_TexCoords; }
    
    static SubTexture CreateFromCoords(MTL::Texture *texture, const simd::float2 &coords, const simd::float2 &spriteSize);
private:
    MTL::Texture* m_Texture;
    
    simd::float2 m_TexCoords[4];
};

#endif /* SubTexture_h */
