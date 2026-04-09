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

Scene::Scene()
{
}

Scene::~Scene()
{
}

Entity Scene::CreateEntity(const char* p_Tag)
{
    Entity entity = { m_Registry.create(), this};

    entity.AddComponent<TransformComponent>();
    auto& entityTag = entity.AddComponent<TagComponent>();
    entityTag.m_Tag = (p_Tag[0] == '\0') ? "Entity" : p_Tag;
    
    return entity;
}

template <typename Renderer>
void Scene::RenderScene(Renderer* p_Renderer, const Camera &p_Camera, const float p_AspectRatio)
{
    auto textured_meshes = m_Registry.view<TransformComponent, MeshComponent, TextureComponent>();
    auto meshes = m_Registry.view<TransformComponent, MeshComponent>();
    
    p_Renderer->BeginScene(p_Camera, p_AspectRatio);
    
    /// TODO: Need to look into iterating all entities in one loop, regardless of whether or not they have a texture component
    for (auto &entity : textured_meshes)
    {
        auto [transform, mesh, textures] = textured_meshes.get<TransformComponent, MeshComponent, TextureComponent>(entity);
        p_Renderer->Render(transform.m_Transform, mesh.m_Mesh, textures.m_Texture);
    }
    
    for (auto &entity : meshes)
    {
        auto [transform, mesh] = textured_meshes.get<TransformComponent, MeshComponent>(entity);
        p_Renderer->Render(transform.m_Transform, mesh.m_Mesh, nullptr);
    }
    
    p_Renderer->EndScene();
}

template void Scene::RenderScene<MetalRenderer>(MetalRenderer*, const Camera &, const float);
