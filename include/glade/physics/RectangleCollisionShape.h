#pragma once

#include <math.h>

#include "glade/math/Vector.h"
#include "glade/math/Transform->h"
#include "CollisionShape.h"

class RectangleCollisionShape : public CollisionShape
{
  public:
  virtual Vector3f testCollisionWith(
    const CollisionShape* target,
    const Transform* selfTransform,
    const Transform* targetTransform
  ) const
  {
    return target->testCollisionShapeWith(this, targetTransform, selfTransform);
  }

  virtual Vector3f testCollisionWith(
    const RectangleCollisionShape* target,
    const Transform* selfTransform,
    const Transform* targetTransform
  ) const
  {
    Vector3f overlap(
      abs(targetTransform->getScale()->x) + abs(selfTransform->getScale()->x) - abs(targetTransform->getPosition()->x - selfTransform->getPosition()->x),
      abs(targetTransform->getScale()->y) + abs(selfTransform->getScale()->y) - abs(targetTransform->getPosition()->y - selfTransform->getPosition()->y),
      0);

    return overlap;
  }
};
