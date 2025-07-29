//
//  SpriteAnimation.cpp
//  LightningCore
//
//  Created by Kian Marvi on 7/1/25.
//

#include "SpriteAnimation.h"

#include "MeshBuilder.h"

#ifdef __APPLE__
#include "../Renderer/Metal/MetalTexture.h"
#endif



SpriteAnimation::SpriteAnimation(const char* p_FileName, MTL::Device* p_MetalDevice, void* p_VertexBuffer)
: m_AnimationCursor(0.f),
  m_CurrentFrameIndex(0),
  m_FramesCount((int)m_Frames.size()),
  m_Speed(0.05f),
  m_SpriteSheet(new MetalTexture(p_FileName, p_MetalDevice))
{
    MeshBuilder::GenerateQuad(p_VertexBuffer);
}

SpriteAnimation::~SpriteAnimation()
{
    if (m_SpriteSheet)
    {
        delete m_SpriteSheet;
        m_SpriteSheet = nullptr;
    }
}

void SpriteAnimation::SetAnimationSpeed(const float p_NewSpeed)
{
    m_Speed = p_NewSpeed;
}

void SpriteAnimation::Play(const float p_DeltaTime)
{ 
    m_AnimationCursor += p_DeltaTime;
    
    if (m_AnimationCursor > m_Speed)
    {
        m_CurrentFrameIndex = (m_CurrentFrameIndex + 1) % m_FramesCount;
        m_AnimationCursor = 0;
    }
    
    simd::float4 p_Frame = m_Frames[m_CurrentFrameIndex];

    // Normalise
    p_Frame.x /= m_SpriteSheet->GetWidth();
    p_Frame.y /= m_SpriteSheet->GetHeight();
    p_Frame.z /= m_SpriteSheet->GetWidth();
    p_Frame.w /= m_SpriteSheet->GetHeight();
}


