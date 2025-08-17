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
    #include "../Renderer/Metal/MetalBuffer.h"
    #include "../Renderer/Metal/MetalShader.h"
#endif



SpriteAnimation::SpriteAnimation(const char* p_FileName, MTL::Device* p_MetalDevice, void* p_VertexBuffer)
: m_Size(100.f),
  m_Rotation(0.f),
  m_Color(Vector4{1.0f, 1.0f, 1.0f, 1.0f}),
  m_SpriteSheet(new MetalTexture(p_FileName, p_MetalDevice)),
  m_Shader(new MetalShader("../LightningGame/Shaders/Sprite.metal", p_MetalDevice, MTL::PixelFormat::PixelFormatDepth32Float))
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
