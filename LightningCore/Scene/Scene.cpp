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
void Scene::RenderScene(Renderer &renderer, const Camera &camera, const float aspectRatio)
{
    renderer.Submit(camera, aspectRatio);
    
    auto textured_meshes = m_Registry.view<TransformComponent, MeshComponent, TextureComponent>();
    auto light_sources = m_Registry.view<TransformComponent, MeshComponent, LightComponent>();
    auto meshes = m_Registry.view<TransformComponent, MeshComponent>(entt::exclude<TextureComponent, LightComponent>);
    
    for (const auto &entity : light_sources)
    {
        const auto &[transform, lights, mesh] = light_sources.get<TransformComponent, LightComponent, MeshComponent>(entity);
        renderer.RenderLights(mul(transform.m_Scale, transform.m_Translation), mesh.m_MeshHandle, lights);
    }
    
    for (const auto &entity : textured_meshes)
    {
        const auto &[transform, textures, mesh] = textured_meshes.get<TransformComponent, TextureComponent, MeshComponent>(entity);
        
        float4x4 scaleMatrix = transform.m_Scale;
        float4x4 rotationMatrix = float4x4::rotation_axis(transform.m_Rotation, transform.m_RotationAngle);
        float4x4 translationMatrix = transform.m_Translation;
        
        float4x4 sr = mul(scaleMatrix, rotationMatrix);
        float4x4 modelMatrix = mul(sr, translationMatrix);
        
        renderer.RenderMesh(modelMatrix, mesh.m_MeshHandle, textures.texture);
    }
    
    for (const auto &entity : meshes)
    {
        const auto &[transform, mesh] = meshes.get<TransformComponent, MeshComponent>(entity);
        
        float4x4 scaleMatrix = transform.m_Scale;
        float4x4 rotationMatrix = float4x4::rotation_axis(transform.m_Rotation, transform.m_RotationAngle);
        float4x4 translationMatrix = transform.m_Translation;
        
        float4x4 sr = mul(scaleMatrix, rotationMatrix);
        float4x4 modelMatrix = mul(sr, translationMatrix);
        
        renderer.RenderMesh(modelMatrix, mesh.m_MeshHandle);
    }
    
    renderer.Commit();
}

template void Scene::RenderScene<MetalRenderer>(MetalRenderer&, const Camera &, const float);
