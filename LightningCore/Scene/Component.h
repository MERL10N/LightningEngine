//
//  Component.h
//  LightningCore
//
//  Created by Kian Marvi on 3/26/26.
//

#ifndef Component_h
#define Component_h
#include <simd/simd.h>

#include "Primitives/MeshBuilder.h"
#include "Camera/Camera.h"

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
    matrix_float4x4 m_Transform = simd::float4x4(1.0f);
    matrix_float4x4 m_Rotation;
    TransformComponent() = default;
    TransformComponent(const TransformComponent&) = default;
    TransformComponent(const simd::float4x4 &transform)
    : m_Transform(transform)
    {}
};

struct TextureComponent
{
    simd::float4 m_Color = simd::make_float4(1.0f, 1.0f, 1.0f, 1.0f);
    
    MetalTexture* m_Texture;
    TextureComponent() = default;
    TextureComponent(const TextureComponent&) = default;
    TextureComponent(const simd::float4 &color)
    : m_Color(color) {}
};

struct MeshComponent
{
    Mesh_3D m_Mesh;
    MeshComponent() = default;
    MeshComponent(const MeshComponent&) = default;
    MeshComponent(const Mesh_3D &mesh)
    : m_Mesh(mesh)
    {}
    
    ~MeshComponent()
    {
        if (m_Mesh.m_Texture)
        {
            delete m_Mesh.m_Texture;
            m_Mesh.m_Texture = nullptr;
        }
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
