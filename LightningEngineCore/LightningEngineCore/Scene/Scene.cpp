//
//  Scene.cpp
//  LightningEngineCore
//
//  Created by Kian Marvi on 9/11/24.
//

#include "Scene.h"
#include <simd/simd.h>

Scene::Scene() 
{
    struct TransformComponent
    {
        simd::float4x4 Transform;
        
        TransformComponent() = default;
        TransformComponent(const TransformComponent&) = default;
        TransformComponent(const simd::float4x4 &transform)
                :Transform(transform) {}
        
        operator simd::float4x4& () { return Transform; }
        operator const simd::float4x4() const { return Transform; }
    };
    flecs::entity entity = registry.entity();
    
    registry.emplace<TransformComponent>(entity, simd::float4x4(1.0f));
    
    //TransformComponent& transform = registry.get<TransformComponent>(entity);
}

Scene::~Scene() 
{
}

