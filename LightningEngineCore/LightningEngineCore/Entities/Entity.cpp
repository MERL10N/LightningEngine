//
//  Entity.cpp
//  LightningEngineCore
//
//  Created by Kian Marvi on 9/12/24.
//

#include "Entity.h"
#include "Scene/Scene.h"

Entity::Entity(entt::entity handle, Scene* scene)
: entityHandle(handle)
, scene(scene)
{
    
}

template<typename T>
bool Entity::HasComponent()
{
    return scene->registry.all_of<T>(entityHandle);
}
