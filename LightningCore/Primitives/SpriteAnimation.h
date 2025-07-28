//
//  SpriteAnimation.h
//  LightningCore
//
//  Created by Kian Marvi on 7/1/25.
//

#ifndef SPRITEANIMATION_H
#define SPRITEANIMATION_H

#include <vector>
#include <stdint.h>

class MetalTexture;
class MetalShader;

class SpriteAnimation
{
public:
    struct SpriteInfo
    {
        uint16_t m_PixelX = 0;
        uint16_t m_PixelY = 0;
        uint16_t m_SpriteRow = 0;
        uint16_t m_SpriteCol = 0;
        uint16_t m_SpriteWidth = 0;
    };
    
    SpriteAnimation(MetalTexture &p_SpriteSheet, uint8_t p_NumSpritesX, uint8_t p_NumSpritesY, float p_WindowWidth, float p_WindowHeight);
    
    void Render(const std::vector<SpriteInfo>& p_Sprites);
    
    void RenderAll();
    
private:
    const char* m_FileName = nullptr;
    
    uint8_t m_NumSpritesX;
    uint8_t m_NumSpritesY;
    
    float m_WindowWidth;
    float m_WindowHeight;
    float m_SpriteAspectRatio;
    float m_TexUSize;
    float m_TexVSize;
    
    MetalTexture &m_SpriteSheet;
};

#endif /* SPRITEANIMATION_H */
