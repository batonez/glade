#pragma once

class CollisionShape
{
public:
  // STATIC и KINEMATIC влияет на то как будут резолвится столкновения. STATIC объекты не в коем случае не могут двигаться
	// Хотя конечно эти свойства должны быть скорее в PhysicBody а не тут. К примеру, есть physicBody - значит это KINEMATIC, нету PhysicBody, значит это статик
  enum CollisionType { STATIC, KINEMATIC };
  
protected:
  CollisionType collisionType;
	bool skipResolve;
	Transform transform;
	
public:
  CollisionShape(CollisionType collisionType) :
  collisionType(STATIC),
  skipResolve(false) // should be in gladeobject
	{}
	
  virtual Vector3f testCollisionWith(
    const CollisionShape* targetShape,
    const Transform* selfTransform,
    const Transform* targetTransform
  ) const = 0;
  
  virtual Vector3f testCollisionWith(
    const RectangleCollisionShape* targetShape,
    const Transform* selfTransform,
    const Transform* targetTransform
  ) const = 0;
  
  CollisionType getType() const
	{
		return collisionType;
	}

	void setType(CollisionType collisionType)
	{
		this->collisionType = collisionType; 
	}

  /*	
  void setTransform(Transform* transform)
  {
    this->transform = transform;
  }*/

  Transform* getTransform()
  {
    return &transform;
  }
};