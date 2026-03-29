//
//  Entity.cpp
//  LightningCore
//
//  Created by Kian Marvi on 12/26/25.
//

#include "Entity.h"
#include "Scene/Scene.h"

Entity::Entity(entt::entity entityHandle, Scene *scene)
: m_EntityHandle(entityHandle),
  m_Scene(scene)
{
    
}

Entity::~Entity()
{
    
}

template<typename T, typename... Args>
T& Entity::AddComponent(Args&&... args)
{
    assert(!HasComponent<T>());
    return m_Scene->Reg().emplace<T>(m_EntityHandle, std::forward<Args>(args)...);
}

template <typename T>
bool Entity::GetComponent()
{
    assert(HasComponent<T>());
    return m_Scene->Reg().get<T>(m_EntityHandle);
}


template <typename T>
bool Entity::HasComponent()
{
    return m_Scene->Reg().all_of<T>(m_EntityHandle);
}

template <typename T>
bool Entity::RemoveComponent()
{
    assert(HasComponent<T>());
    return m_Scene->Reg().remove<T>(m_EntityHandle);
}
