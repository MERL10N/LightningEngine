//
//  Scene.cpp
//  LightningCore
//
//  Created by Kian Marvi on 12/26/25.
//

#include "Scene.h"
#include "Component.h"
#include "Entity/Entity.h"
#include <simd/simd.h>

void Scene::DoMath(const simd::float4x4 &transform)
{
}

void Scene::OnTransformConstruct(entt::registry &registry, entt::entity entity)
{
}

Scene::Scene()
{
    /*
    m_Registry.emplace<TransformComponent>(m_Entity, simd::float4x4(1.0f));
    m_Registry.on_construct<TransformComponent>().connect<&OnTransformConstruct>();
    
    if (m_Registry.all_of<TransformComponent>(m_Entity))
        TransformComponent &transform = m_Registry.get<TransformComponent>(m_Entity);
    
    auto view = m_Registry.view<TransformComponent>();
    
    for (auto &entity : view)
    {
        TransformComponent &transform = view.get<TransformComponent>(entity);
    }
    
    auto group = m_Registry.group<TransformComponent>(entt::get<SpriteRendererComponent>);
    for (auto &entity : group)
    {
        auto[transform, sprite] = group.get<TransformComponent, SpriteRendererComponent>(entity);
    }
     */
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

void Scene::Update(float deltaTime)
{
}
