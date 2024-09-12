//
//  Entity.cpp
//  LightningEngineCore
//
//  Created by Kian Marvi on 9/11/24.
//

#include "EntityManager.h"

EntityManager::EntityManager()
: entityWorld()
{
}

flecs::entity EntityManager::CreateEntity(const char *tag)
{
    auto entity = entityWorld.entity(tag);
    return entity;
}

void EntityManager::Run() 
{
    entityWorld.progress();
}


template<typename T>
void EntityManager::RegisterComponent(const char* name)
{
    entityWorld.component<T>(name);
}

template<typename T>
T& EntityManager::AddComponent(flecs::entity &entity)
{
    entity.set<T>();
}

template<typename T>
T* EntityManager::GetComponent(flecs::entity &entity)
{
    entity.get<T>();
}
