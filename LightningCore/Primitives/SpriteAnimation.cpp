//
//  SpriteAnimation.cpp
//  LightningCore
//
//  Created by Kian Marvi on 7/1/25.
//

#include "SpriteAnimation.h"

#ifdef __APPLE__
#include "../Renderer/Metal/MetalTexture.h"
#endif



SpriteAnimation::SpriteAnimation(MetalTexture &p_SpriteSheet, uint8_t p_NumSpritesX, uint8_t p_NumSpritesY, float p_WindowWidth, float p_WindowHeight)
: m_NumSpritesX(p_NumSpritesX),
  m_NumSpritesY(p_NumSpritesY),
  m_WindowWidth(p_WindowWidth),
  m_WindowHeight(p_WindowHeight),
  m_SpriteSheet(p_SpriteSheet)
{
    
}
