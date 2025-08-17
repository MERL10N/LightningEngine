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
#include "../Math/LightningMath.h"

#ifdef __APPLE__
    class MetalTexture;
    class MeshBuilder;
    class MetalShader;

    namespace MTL
    {
        class Device;
    }
#endif

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
    
    inline void SetID(uint8_t p_ID)                    { m_ID = p_ID; }
    inline void SetPosition(const Vector2 &p_Position) { m_Position = p_Position; }
    inline void SetSize(const Vector2 &p_Size)         { m_Size = p_Size; }
    inline void SetColor(const Vector4 &p_Color)       { m_Color = p_Color; }
    inline void SetRotation(const float p_Rotation)    { m_Rotation = p_Rotation; }
    
    inline uint8_t GetID()       { return m_ID; }
    inline Vector2 GetPosition() { return m_Position; }
    inline float   GetRotation() { return m_Rotation; }
    inline Vector2 GetSize()     { return m_Size; }
    inline Vector4 GetColor()    { return m_Color; }

    
private:
    const char* m_FileName = nullptr;
    
    Vector2 m_Position;
    Vector2 m_Size;
    Vector4 m_Color;
    
    float m_Rotation;
    uint8_t m_ID;

    MetalTexture *m_SpriteSheet = nullptr;
    MetalShader* m_Shader = nullptr;
};

#endif /* SPRITEANIMATION_H */
