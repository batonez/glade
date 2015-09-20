#pragma once

#include "CollisionEvent.h"

class CollisionEventListener
{
  public:
    virtual bool onCollision(CollisionEvent &collisionEvent) = 0;
};
