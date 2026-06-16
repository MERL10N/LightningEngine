//
//  Sprite.hpp
//  LightningCore
//
//  Created by Kian Marvi on 3/25/26.
//

#ifndef Sprite_h
#define Sprite_h

#include <stdint.h>

struct SpriteUniform
{
    uint32_t m_CurrentFrame;
    uint32_t m_Rows;
    uint32_t m_Columns;
    uint32_t m_Padding;
};

class Sprite
{
public:
    Sprite() = default;
    Sprite(const uint32_t rows, const uint32_t columns, const float fps, const bool isAnimated);
    
    void Update(const float deltaTime);
    
    SpriteUniform GetUniforms() const;
private:
    uint32_t m_TotalFrames = 1;
    uint32_t m_CurrentFrame = 0;
    uint32_t m_Rows = 0;
    uint32_t m_Columns = 0;
    float m_FrameDuration = 0.f;
    float m_CurrentTime = 0.f;
    float m_FPS = 0.0f;
    bool b_IsAnimated = false;
};

#endif /* Sprite_hpp */
