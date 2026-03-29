//
//  Scene.h
//  LightningCore
//
//  Created by Kian Marvi on 12/26/25.
//

#ifndef Scene_h
#define Scene_h

#include "entt/single_include/entt/entt.hpp"
#include <simd/simd.h>

class Entity;

class Scene
{
public:
    Scene();
    ~Scene();
    
    Entity CreateEntity(const char* tag);
    
    // Temporary
    entt::registry &Reg() { return m_Registry; }
    void Update(float deltaTime);
    
private:
    static void DoMath(const simd::float4x4 &transform);
    static void OnTransformConstruct(entt::registry &registry, entt::entity entity);
    
    entt::registry m_Registry;
    entt::entity m_Entity;
};

#endif /* Scene_h */
