#pragma once

#include "CollisionEvent.h"

class CollisionEventListener
{
	virtual bool onCollision(CollisionEvent &collisionEvent) = 0;
};
