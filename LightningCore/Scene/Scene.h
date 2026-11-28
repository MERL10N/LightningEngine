//
//  Scene.h
//  LightningCore
//
//  Created by Kian Marvi on 12/26/25.
//

#ifndef Scene_h
#define Scene_h

#include <entt/entt.hpp>
#include <box3d/box3d.h>

class Entity;
class Camera;

class Scene
{
public:
    Scene();
    ~Scene();
    
    Entity CreateEntity(const char* tag);
    
    // Temporary
    entt::registry &Reg() { return m_Registry; }
    
    void OnRuntimeStart();
    void OnUpdateRuntime(float dt);
    void OnRuntimeStop();
    
    template <typename Renderer>
    void RenderScene(Renderer &p_Renderer, const float p_AspectRatio);
    
private:    
    entt::registry  m_Registry;
    entt::entity    m_Entity;
    
    b3WorldDef      m_PhysicsWorld;
    b3WorldId       m_WorldId;
};

#endif /* Scene_h */
