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

Entity Scene::CreateEntity(const char* tag)
{
    Entity entity = { m_Registry.create(), this};

    entity.AddComponent<TransformComponent>();
    auto& entityTag = entity.AddComponent<TagComponent>();
    entityTag.m_Tag = (tag[0] == '\0') ? "Entity" : tag;
    
    return entity;
}

void Scene::RenderScene(MetalRenderer* p_MetalRenderer, const Camera &p_Camera, const float p_AspectRatio)
{
    auto meshes = m_Registry.view<TransformComponent, MeshComponent>();
    
    p_MetalRenderer->BeginScene(p_Camera, p_AspectRatio);
    
    for (auto &entity : meshes)
    {
        auto [transform, mesh] = meshes.get<TransformComponent, MeshComponent>(entity);
        p_MetalRenderer->Render(transform.m_Transform, mesh.m_Mesh);
    }
    
    p_MetalRenderer->EndScene();
}
