//
//  Entity.cpp
//  LightningCore
//
//  Created by Kian Marvi on 12/26/25.
//

#include "Entity.h"

Entity::Entity(entt::entity entityHandle, Scene *scene)
: m_EntityHandle(entityHandle),
  m_Scene(scene)
{
    
}

Entity::~Entity()
{
    
}


