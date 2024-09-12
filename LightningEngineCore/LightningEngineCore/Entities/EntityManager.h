//
//  EntityManager.h
//  LightningEngineCore
//
//  Created by Kian Marvi on 9/11/24.
//

#ifndef EntityManager_h
#define EntityManager_h
#include "ThirdParty/ECS/flecs.h"

class Scene;

class EntityManager
{
public:
    EntityManager();
    
    flecs::entity CreateEntity(const char* tag);
    
    // Register a new component type
    template<typename T>
    void RegisterComponent(const char* name);
    
    // Add a component to an entity
    template<typename T>
    T& AddComponent(flecs::entity &entity);
    
    // Get the component from an entity
    template<typename T>
    T* GetComponent(flecs::entity &entity);
    
    void Run();
    
private:
    flecs::world entityWorld;
    Scene* scene;
};

#endif /* EntityManager_h */
