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
#include "MeshBuilder.h"
#include <Metal/Metal.hpp>
#endif



SpriteAnimation::SpriteAnimation(const char* p_FileName, MTL::Device* p_Device, const Vector2 &p_Position, const Vector2 &p_Size, float p_Rotation)
: m_Size(p_Size),
  m_Rotation(p_Rotation),
  m_Color(Vector4{1.0f, 1.0f, 1.0f, 1.0f}),
  m_Device(p_Device),
  m_SpriteSheet(new MetalTexture(p_FileName)),
  m_Shader(new MetalShader("../LightningGame/Shaders/Sprite.metal", m_Device ,"vertexShader", "fragmentShader"))
{
    m_SpriteSheet->SetMetalDevice(m_Device);
}

SpriteAnimation::~SpriteAnimation()
{
    if (m_SpriteSheet)
    {
        delete m_SpriteSheet;
        m_SpriteSheet = nullptr;
    }
    if (m_Shader)
    {
        delete m_Shader;
    }
    if (m_Device)
    {
        m_Device->release();
        m_Device = nullptr;
    }
}
