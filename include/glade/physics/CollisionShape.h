#pragma once

#include "glade/math/Transform.h"

class RectangleCollisionShape;

class CollisionShape
{
public:
  // STATIC и KINEMATIC влияет на то как будут резолвится столкновения. STATIC объекты не в коем случае не могут двигаться
  // Хотя конечно эти свойства должны быть скорее в PhysicBody а не тут. Или, к примеру, есть physicBody - значит это KINEMATIC, нету PhysicBody, значит это статик
  enum CollisionType { STATIC, KINEMATIC };

protected:
  CollisionType collisionType;
  Transform transform;

public:
  CollisionShape(CollisionType collisionType) :
    collisionType(STATIC)
  {}

  virtual bool testCollisionWith(
    const CollisionShape* targetShape,
    const Transform* selfTransform,
    const Transform* targetTransform,
    Vector3f &overlap /*out*/
  ) const = 0;

  virtual bool testCollisionWith(
    const RectangleCollisionShape* targetShape,
    const Transform* selfTransform,
    const Transform* targetTransform,
    Vector3f &overlap /*out*/
  ) const = 0;

  CollisionType getType() const
  {
    return collisionType;
  }

  void setType(CollisionType collisionType)
  {
    this->collisionType = collisionType;
  }

  Transform* getTransform()
  {
    return &transform;
  }
};