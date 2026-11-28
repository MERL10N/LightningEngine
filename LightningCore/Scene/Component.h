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
#endif

#include <hlsl++/vector_float_type.h>
#include <hlsl++/matrix_float_type.h>
using namespace hlslpp;


class TagComponent
{
public:
    const char* m_Tag;
    
    TagComponent() = default;
    TagComponent(const TagComponent&) = default;
    TagComponent(const char* tag)
    : m_Tag(tag)
    {}
};

class TransformComponent
{
public:
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
    
    inline const float     GetRotationAngle()  const { return m_RotationAngle; }
    inline const float4x4& GetTranslation()    const { return m_Translation; }
    inline const float4x4& GetScale()          const { return m_Scale; }
    inline const float3&   GetRotation()       const { return m_Rotation; }
    
private:
    float4x4 m_Translation  = float4x4::identity();
    float3   m_Rotation     = float3(0.0f, 1.0f, 0.0f);
    float4x4 m_Scale        = float4x4::scale(1.0f, 1.0f, 1.0f);
    
    float m_RotationAngle = 0.0f;
    
};

class TextureComponent
{
public:
    TextureComponent() = default;
    TextureComponent(const TextureComponent&) = delete;
    TextureComponent& operator=(const TextureComponent&) = delete;
    
    // For a single texture map
    template<typename Device>
    TextureComponent(const char *filePath, Device* device)
    : texture(filePath, device)
    {
    }
    
    // For multiple texture maps
    template<typename Device>
    TextureComponent(const std::vector<const char*> &filePaths, Device* device)
    : texture(filePaths, device)
    {
    }
    
    
    template<typename Device>
    TextureComponent(const std::array<const char*, 6> & faces, Device* device)
    : texture(faces, device)
    {
    }
    
    ~TextureComponent()
    {
    }
    
    const Texture& GetTexture() const { return texture; }
    
private:
    Texture texture;
};

class MeshComponent
{
public:

    MeshComponent() = default;
    MeshComponent(const MeshComponent&) = default;
    MeshComponent(const MeshHandle mesh)
    : m_MeshHandle(mesh)
    {}
    MeshComponent(const MeshHandle mesh, const std::vector<float4x4> &m_Transforms)
    : m_MeshHandle(mesh),
      m_Transforms(std::move(m_Transforms))
    {}
    
    inline const MeshHandle& GetMeshHandle() const { return m_MeshHandle; }
    inline const std::vector<float4x4>& GetTransforms() const { return m_Transforms; }
    
private:
    MeshHandle m_MeshHandle;
    std::vector<float4x4> m_Transforms;
};

class LightComponent
{
  
public:
    LightComponent() = default;
    LightComponent(const LightComponent&) = default;
    LightComponent(const float3& color)
    : m_Color(color)
    {
    }
    
    LightComponent(const float3& color, const float3 &position)
    : m_Color(color),
      m_Position(position)
    {
    }
    
    inline const float3& GetColor()    const { return m_Color; }
    inline const float3& GetPosition() const { return m_Position; }
    
private:
    float3 m_Color      = float3(1.0f, 1.0f, 1.0f);
    float3 m_Position   = float3(0.0f, 0.0f, 0.0f);
};

struct SkyboxComponent
{
    bool m_Active = true;
};

class CameraComponent
{
public:
    CameraComponent() = default;
    CameraComponent(const CameraComponent&) = default;
    CameraComponent& operator= (const CameraComponent& other) = default;
    CameraComponent(const Camera &camera)
    : m_Camera(camera)
    {}
    
    Camera& GetCamera() { return m_Camera; }
    
    inline bool GetPrimaryCamera() const { return b_Primary; }
    inline void SetAsPrimaryCamera(const bool primary) { b_Primary = primary; }
    
private:
    Camera m_Camera;
    bool   b_Primary = false;
    
};

class RigidBody3DComponent
{
public:
    enum class BodyType { Static = 0, Dynamic, Kinematic };
    
    inline BodyType GetRigidBodyType() const { return m_Type; }
    
    inline bool IsFixedRotation() const { return b_FixedRotation; }
    
    RigidBody3DComponent() = default;
    RigidBody3DComponent(const RigidBody3DComponent&) = default;
    
    
private:
    BodyType m_Type             = BodyType::Static;
    float    m_Mass             = 1.0f;
    float    m_LinearDamping    = 1.0f;
    bool     b_FixedRotation    = false;
    
};

class BoxCollider3DComponent
{
    
};

#endif /* Component_h */
