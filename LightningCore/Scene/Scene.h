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
class Camera;

class MetalRenderer;

class Scene
{
public:
    Scene();
    ~Scene();
    
    Entity CreateEntity(const char* tag);
    
    // Temporary
    entt::registry &Reg() { return m_Registry; }
    template <typename Renderer>
    void RenderScene(Renderer* p_Renderer, const Camera &p_Camera, const float p_AspectRatio);
    
private:    
    entt::registry m_Registry;
    entt::entity m_Entity;
};

#endif /* Scene_h */
