#pragma once

#include <math.h>

#include "glade/math/Vector.h"
#include "glade/math/Transform.h"
#include "CollisionShape.h"

class RectangleCollisionShape : public CollisionShape
{
  public:
    RectangleCollisionShape(CollisionShape::CollisionType type):
      CollisionShape(type)
    {}
  
  virtual bool testCollisionWith(
    const CollisionShape* target,
    const Transform* selfTransform,
    const Transform* targetTransform,
    Glade::Vector3f &overlap
  ) const
  {
    return target->testCollisionWith(this, targetTransform, selfTransform, overlap);
  }

  virtual bool testCollisionWith(
    const RectangleCollisionShape* target,
    const Transform* selfTransform,
    const Transform* targetTransform,
    Glade::Vector3f &overlap
  ) const
  {
    float overlapX = abs(targetTransform->getScale()->x) + abs(selfTransform->getScale()->x) - abs(targetTransform->getPosition()->x - selfTransform->getPosition()->x);
    float overlapY = abs(targetTransform->getScale()->y) + abs(selfTransform->getScale()->y) - abs(targetTransform->getPosition()->y - selfTransform->getPosition()->y);
    
    if (overlapX > 0 && overlapY > 0) {
      overlap.x = overlapX;
      overlap.y = overlapY;
      overlap.z = 0;
      
      return true;
    }
    
    return false;
  }
};
