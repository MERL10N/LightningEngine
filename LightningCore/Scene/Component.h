//
//  Component.h
//  LightningCore
//
//  Created by Kian Marvi on 3/26/26.
//

#ifndef Component_h
#define Component_h
#include <simd/simd.h>

class MetalTexture;
struct TagComponent
{
    const char* m_Tag;
    
    TagComponent() = default;
    TagComponent(const TagComponent&) = default;
    TagComponent(const char* tag)
    : m_Tag(tag)
    {}
};

struct TransformComponent
{
    simd::float4x4 m_Transform;
    
    TransformComponent() = default;
    TransformComponent(const TransformComponent&) = default;
    TransformComponent(const simd::float4x4 &transform)
    : m_Transform(transform)
    {}
    
    operator simd::float4x4() { return m_Transform; }
    operator const simd::float4x4&() const { return m_Transform; }
};

struct SpriteRendererComponent
{
    simd::float4 m_Color = simd::make_float4(1.0f, 1.0f, 1.0f, 1.0f);
    
    MetalTexture* m_Texture;
    SpriteRendererComponent() = default;
    SpriteRendererComponent(const SpriteRendererComponent&) = default;
    SpriteRendererComponent(const simd::float4 &color)
    : m_Color(color) {}
};

#endif /* Component_h */
