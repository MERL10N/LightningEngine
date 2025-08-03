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

#include <simd/simd.h>
class MetalTexture;
class MeshBuilder;

namespace MTL
{
    class Device;
}

enum AnimationType
{
    IDLE = 0,
    PLAY,
    JUMP,
    ATTACK
};

class SpriteAnimation
{
public:
    SpriteAnimation(const char* p_FileName, MTL::Device* p_MetalDevice, void* p_VertexBuffer);
    ~SpriteAnimation();
    
    void Play(const float p_DeltaTime);
    
    void SetAnimationSpeed(const float p_NewSpeed);
    
private:
    const char* m_FileName = nullptr;
    
    int m_CurrentFrameIndex;
    int m_FramesCount;

    float m_Speed;
    float m_AnimationCursor;
    
    std::vector<simd::float4> m_Frames;
    MetalTexture *m_SpriteSheet = nullptr;
};

#endif /* SPRITEANIMATION_H */
