//
//  Entity.h
//  LightningCore
//
//  Created by Kian Marvi on 12/26/25.
//

#ifndef Entity_h
#define Entity_h

#include "Scene/Scene.h"
#include "entt/single_include/entt/entt.hpp"

class Entity
{
public:
    Entity() = default;
    Entity(const entt::entity &entityHandle, Scene *scene);
    Entity(const Entity& other) = default;
    ~Entity();
    
    template<typename T, typename... Args>
    T& AddComponent(Args&&... args)
    {
        assert(!HasComponent<T>());
        return m_Scene->Reg().emplace<T>(m_EntityHandle, std::forward<Args>(args)...);
    }
    
    template <typename T>
    bool HasComponent()
    {
        return m_Scene->Reg().all_of<T>(m_EntityHandle);
    }
    
    template <typename T>
    bool GetComponent()
    {
        assert(HasComponent<T>());
        return m_Scene->Reg().get<T>(m_EntityHandle);
    }
    
    template <typename T>
    bool RemoveComponent()
    {
        return m_Scene->Reg().remove<T>(m_EntityHandle);
    }
private:
    entt::entity m_EntityHandle;
    Scene *m_Scene;
};

#endif /* Entity_h */
