//
//  Scene.cpp
//  LightningCore
//
//  Created by Kian Marvi on 12/26/25.
//

#include "Scene.h"
#include "Component.h"
#include "Entity/Entity.h"
#include "Renderer/Metal/MetalRenderer.h"
#include "Camera/Camera.h"
#include <simd/simd.h>
#include "Math/AAPLMathUtilities.h"

Scene::Scene()
{
}

Scene::~Scene()
{
}

Entity Scene::CreateEntity(const char* p_Tag)
{
    Entity entity = { m_Registry.create(), this};

    auto& entityTag = entity.AddComponent<TagComponent>();
    entityTag.m_Tag = (p_Tag[0] == '\0') ? "Entity" : p_Tag;
    
    return entity;
}

template <typename Renderer>
void Scene::RenderScene(Renderer &p_Renderer, const Camera &p_Camera, const float p_AspectRatio)
{
    p_Renderer.Submit(p_Camera, p_AspectRatio);
    
    auto textured_meshes = m_Registry.view<TransformComponent, MeshComponent, TextureComponent>();
    auto light_sources = m_Registry.view<TransformComponent, MeshComponent, LightComponent>();
    auto meshes = m_Registry.view<TransformComponent, MeshComponent>(entt::exclude<TextureComponent, LightComponent>);
    
    for (const auto &entity : light_sources)
    {
        auto [transform, lights, mesh] = light_sources.get<TransformComponent, LightComponent, MeshComponent>(entity);
        p_Renderer.RenderLights(matrix_multiply(transform.m_Transform, transform.m_Scale), mesh.m_Mesh, lights);
    }
    
    for (const auto &entity : textured_meshes)
    {
        auto [transform, mesh, textures] = textured_meshes.get<TransformComponent, MeshComponent, TextureComponent>(entity);
        
        matrix_float4x4 rotationMatrix = matrix4x4_rotation(transform.m_RotationAngle, transform.m_Rotation.x, transform.m_Rotation.y, transform.m_Rotation.z);
        
        matrix_float4x4 rs = matrix_multiply(rotationMatrix, transform.m_Scale);
        matrix_float4x4 modelMatrix = matrix_multiply(transform.m_Transform, rs);
        p_Renderer.RenderMesh(modelMatrix, mesh.m_Mesh, textures.GetTexture());
    }
    
    for (const auto &entity : meshes)
    {
        auto [transform, mesh] = meshes.get<TransformComponent, MeshComponent>(entity);
        
        matrix_float4x4 rotationMatrix = matrix4x4_rotation(transform.m_RotationAngle, transform.m_Rotation.x, transform.m_Rotation.y, transform.m_Rotation.z);
        
        matrix_float4x4 rs = matrix_multiply(rotationMatrix, transform.m_Scale);
        matrix_float4x4 modelMatrix = matrix_multiply(transform.m_Transform, rs);
        p_Renderer.RenderMesh(modelMatrix, mesh.m_Mesh, nullptr);
    }
    
    p_Renderer.Commit();
}

template void Scene::RenderScene<MetalRenderer>(MetalRenderer&, const Camera &, const float);
