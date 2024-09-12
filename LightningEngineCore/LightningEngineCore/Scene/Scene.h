//
//  Scene.h
//  LightningEngineCore
//
//  Created by Kian Marvi on 9/11/24.
//

#ifndef Scene_h
#define Scene_h

#include <ThirdParty/entt/entt.hpp>

class Scene
{
public:
    Scene();
    ~Scene();
    
    entt::entity CreateEntity();
private:
    entt::registry registry;
};

#endif /* Scene_h */
