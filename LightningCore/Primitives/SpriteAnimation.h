//
//  SpriteAnimation.h
//  LightningCore
//
//  Created by Kian Marvi on 7/1/25.
//

#ifndef SPRITEANIMATION_H
#define SPRITEANIMATION_H

#include <vector>
class MetalTexture;

class SpriteAnimator
{
public:
    SpriteAnimator(const MetalTexture* p_SpriteSheet, int p_Frames, int p_Speed);
private:
    std::vector<int> m_Frames;
    int m_RepeatCount = 0;
    float m_AnimationTime = 0.f;

    bool b_HasEnded = false;
    bool b_IsAnimationActive = false;
};

#endif /* SPRITEANIMATION_H */
