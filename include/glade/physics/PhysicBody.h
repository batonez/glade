#pragma once

#include "../math/Vector.h"

class PhysicBody
{
  public:
    Vector3f velocity;
    Vector3f acceleration;
    bool affectedByGravity;

    PhysicBody(): affectedByGravity(true) {}
    
    void stepSimulation(double deltaTime, Transform* transform)
    {
      velocity.x += deltaTime * acceleration.x;		
      velocity.y += deltaTime * acceleration.y;
      velocity.z += deltaTime * acceleration.z;
      
      transform->getPosition().x += deltaTime * velocity.x;
      transform->getPosition().y += deltaTime * velocity.y;
      transform->getPosition().z += deltaTime * velocity.z;
    }
};
