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
#include <hlsl++.h>

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
    /*
    {
        auto group = m_Registry.view<CameraComponent, TransformComponent>();
        for (const auto &entity : group)
        {
            auto &[transform, camera] = m_Registry.get<TransformComponent>(entity).m_Transform;
        }
    }*/
    
    p_Renderer.Submit(p_Camera, p_AspectRatio);
    
    auto textured_meshes = m_Registry.view<TransformComponent, MeshComponent, TextureComponent>();
    auto light_sources = m_Registry.view<TransformComponent, MeshComponent, LightComponent>();
    auto meshes = m_Registry.view<TransformComponent, MeshComponent>(entt::exclude<TextureComponent, LightComponent>);
    
    for (const auto &entity : light_sources)
    {
        auto [transform, lights, mesh] = light_sources.get<TransformComponent, LightComponent, MeshComponent>(entity);
        p_Renderer.RenderLights(mul(transform.m_Scale, transform.m_Translation), mesh.m_Mesh, lights);
    }
    
    for (const auto &entity : textured_meshes)
    {
        auto [transform, mesh, textures] = textured_meshes.get<TransformComponent, MeshComponent, TextureComponent>(entity);
        
        float4x4 scaleMatrix = transform.m_Scale;
        float4x4 rotationMatrix = float4x4::rotation_axis(transform.m_Rotation, transform.m_RotationAngle);
        float4x4 translationMatrix = transform.m_Translation;
        
        float4x4 sr = mul(scaleMatrix, rotationMatrix);
        float4x4 modelMatrix = mul(sr, translationMatrix);
        
        p_Renderer.RenderMesh(modelMatrix, mesh.m_Mesh, textures.m_Texture);
    }
    
    for (const auto &entity : meshes)
    {
        auto [transform, mesh] = meshes.get<TransformComponent, MeshComponent>(entity);
        
        float4x4 scaleMatrix = transform.m_Scale;
        float4x4 rotationMatrix = float4x4::rotation_axis(transform.m_Rotation, transform.m_RotationAngle);
        float4x4 translationMatrix = transform.m_Translation;
        
        float4x4 sr = mul(scaleMatrix, rotationMatrix);
        float4x4 modelMatrix = mul(sr, translationMatrix);
        
        p_Renderer.RenderMesh(modelMatrix, mesh.m_Mesh, nullptr);
    }
    
    p_Renderer.Commit();
}

template void Scene::RenderScene<MetalRenderer>(MetalRenderer&, const Camera &, const float);
