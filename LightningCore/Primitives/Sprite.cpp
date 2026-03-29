//
//  Sprite.cpp
//  LightningCore
//
//  Created by Kian Marvi on 3/25/26.
//

#include "Sprite.h"
#include <iostream>

Sprite::Sprite(const uint32_t rows, const uint32_t columns, const float fps, const bool isAnimated)
: m_Rows(rows),
  m_Columns(columns),
  b_IsAnimated(isAnimated),
  m_FPS(fps)
{
    m_TotalFrames = m_Rows * m_Columns;
     
    if (m_FPS > 0.0f)
    {
         m_FrameDuration = 1.0 / m_FPS;
    }
    else
    {
         m_FrameDuration = 0.0f;
    }
}

SpriteUniform Sprite::GetUniforms() const
{
    return SpriteUniform(m_CurrentFrame, m_Rows, m_Columns, 0);
}

void Sprite::Update(const float deltaTime)
{
    if (!b_IsAnimated || m_FrameDuration <= 0.0f)
    {
        return;
    }
    
    m_CurrentTime += deltaTime;
    if (m_CurrentTime >= m_FrameDuration)
    {
        m_CurrentFrame = (m_CurrentFrame + 1) % m_TotalFrames;
        m_CurrentTime -= m_FrameDuration;
    }
    
    std::println("Current Frame: {}", m_CurrentFrame);
}


