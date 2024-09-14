//
//  Scene.h
//  LightningEngineCore
//
//  Created by Kian Marvi on 9/11/24.
//

#ifndef Scene_h
#define Scene_h

#include "entt.hpp"

class Entity;

class Scene
{
public:
    Scene();
    ~Scene();
    
   Entity CreateEntity();
private:
    entt::registry registry;
    friend class Entity;
};

#endif /* Scene_h */
