//
//  Entity.h
//  LightningEngineCore
//
//  Created by Kian Marvi on 9/12/24.
//

#ifndef Entity_h
#define Entity_h

#include "entt.hpp"
class Scene;

class Entity
{
public:
    Entity(entt::entity handle, Scene* scene);
    Entity(const Entity& other) = default;
    
    template<typename T, typename... Args>
    T& AddComponent(Args&& ...args)
    {
       // return scene->registry.emplace<T>(entityHandle, std::forward<Args>(args)...);
    }
    
    template<typename T>
    T& GetComponent()
    {
       // return scene->registry.has<T>(entityHandle);
    }

    
    template<typename T>
    bool HasComponent();
    
private:
    entt::entity entityHandle;
    Scene* scene;
};

#endif /* Entity_h */
