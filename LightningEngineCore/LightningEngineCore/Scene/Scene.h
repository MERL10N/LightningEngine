//
//  Scene.h
//  LightningEngineCore
//
//  Created by Kian Marvi on 9/11/24.
//

#ifndef Scene_h
#define Scene_h

#include "ThirdParty/ECS/flecs.h"

class Scene
{
    Scene();
    ~Scene();
private:
    flecs::world registry;
};

#endif /* Scene_h */
