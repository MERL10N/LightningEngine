//
//  Entity.h
//  LightningCore
//
//  Created by Kian Marvi on 12/26/25.
//

#ifndef Entity_h
#define Entity_h

class Scene;

#include "entt/single_include/entt/entt.hpp"

class Entity
{
public:
    Entity() = default;
    Entity(entt::entity entityHandle, Scene *scene);
    Entity(const Entity& other) = default;
    ~Entity();
    
    template<typename T, typename... Args>
    T& AddComponent(Args&&... args);
    
    template <typename T>
    bool HasComponent();
    
    template <typename T>
    bool GetComponent();
    
    template <typename T>
    bool RemoveComponent();
private:
    entt::entity m_EntityHandle;
    Scene *m_Scene;
};

#endif /* Entity_h */
