//
//  Scene.cpp
//  LightningEngineCore
//
//  Created by Kian Marvi on 9/11/24.
//

#include "Scene.h"
#include "Entities/Entity.h"
#include <simd/simd.h>

Scene::Scene() 
{
    struct MeshComponent
    {
        bool data = false;
        float value = 0.f;
        MeshComponent() = default;
    };
    struct TransformComponent
    {
        simd::float4x4 Transform;
        
        TransformComponent() = default;
        TransformComponent(const TransformComponent&) = default;
        TransformComponent(const simd::float4x4& transform)
        :Transform(transform) {}
        
        operator simd::float4x4& () { return Transform; }
        operator const simd::float4x4& () { return Transform; }
    };
    
    entt::entity entity = registry.create();
    
    registry.emplace<TransformComponent>(entity, simd::float4x4(1.0f));
    
    if (registry.all_of<TransformComponent>(entity))
        TransformComponent& transform = registry.get<TransformComponent>(entity);
    
    auto view = registry.view<TransformComponent>();
    for (auto entity : view)
    {
        TransformComponent &transform = view.get<TransformComponent>(entity);
    }
    
    auto group = registry.group<TransformComponent>(entt::get<MeshComponent>);
    
    for (auto entity : group)
    {
        //auto&[transform, mesh] = group.get<TransformComponent, MeshComponent>(entity);
    }
    
}

Scene::~Scene() 
{
}

Entity Scene::CreateEntity()
{
    return {registry.create(), this};
}


