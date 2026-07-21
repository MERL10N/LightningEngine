//
//  Component.h
//  LightningCore
//
//  Created by Kian Marvi on 3/26/26.
//

#ifndef Component_h
#define Component_h
#include "Primitives/MeshBuilder.h"
#include "Camera/Camera.h"

#ifdef __APPLE__
    #include "Renderer/Metal/MetalTexture.h"
    using Texture = MetalTexture;
    using Device  = MTL::Device;
    using Buffer  = MTL::Buffer;
#endif
/*
#elif __WIN32__
    // using Device = VkPhysicalDevice;
    // using Texture = VulkanTexture;
#endif
 */

#include <hlsl++.h>
using namespace hlslpp;


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
    float4x4 m_Translation  = float4x4::identity();
    float3   m_Rotation     = float3(0.0f, 1.0f, 0.0f);
    float4x4 m_Scale        = float4x4::scale(1.0f, 1.0f, 1.0f);
    
    float m_RotationAngle = 0.0f;
    TransformComponent() = default;
    TransformComponent(const TransformComponent&) = default;
    TransformComponent(const float3 &transform)
    : m_Translation(float4x4::translation(transform))
    {}
    
    TransformComponent(const float3 &transform, const float3 &scale)
    : m_Translation(float4x4::translation(transform)),
      m_Scale(float4x4::scale(scale.x, scale.y, scale.z))
    {}
    
    TransformComponent(const float3 &transform, const float &angle, const float3 &rotationAxis)
    : m_Translation(float4x4::translation(transform)),
      m_RotationAngle((angle * M_PI) / 180.f),
      m_Rotation(rotationAxis)
    {
    }
};

struct TextureComponent
{
    Texture* texture = nullptr;
    TextureComponent() = default;
    TextureComponent(const TextureComponent&) = delete;
    TextureComponent& operator=(const TextureComponent&) = delete;
    
    TextureComponent(const char *filePath, Device* device)
    : texture(new Texture(filePath, device))
    {}

    TextureComponent (TextureComponent&& pOther)
    {
        texture = pOther.texture;
        pOther.texture = nullptr;
    }
    
    TextureComponent& operator=(TextureComponent&& pOther)
    {
        if (this != &pOther)
        {
            delete texture;
            texture = pOther.texture;
            pOther.texture = nullptr;
        }
        return (*this);
    }
    
    ~TextureComponent()
    {
        if (texture)
        {
            delete texture;
            texture = nullptr;
        }
    }
};

struct MeshComponent
{
    MeshHandle m_MeshHandle;
    MeshComponent() = default;
    MeshComponent(const MeshComponent&) = default;
    MeshComponent(const MeshHandle mesh)
    : m_MeshHandle(mesh)
    {}
};

struct LightComponent
{
    float3 m_Color = float3(1.0f, 1.0f, 1.0f);
    float3 m_Position = float3(0.0f, 0.0f, 0.0f);
    LightComponent() = default;
    LightComponent(const LightComponent&) = default;
    LightComponent(const float3& color)
    : m_Color(color)
    {
    }
};

struct CameraComponent
{
    Camera m_Camera;
    bool   b_Primary = true;
    CameraComponent() = default;
    CameraComponent(const CameraComponent&) = default;
    CameraComponent(const Camera &camera)
    : m_Camera(camera)
    {}
};

#endif /* Component_h */
