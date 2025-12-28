//
//  Scene.h
//  LightningCore
//
//  Created by Kian Marvi on 12/26/25.
//

#ifndef Scene_h
#define Scene_h

#include "entt/single_include/entt/entt.hpp"

class Scene
{
public:
    Scene();
    ~Scene();
private:
    entt::registry m_Registry;
};

#endif /* Scene_h */
