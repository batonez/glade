#pragma once

#include "../math/Vector.h"

class PhysicBody
{
  public:
    Vector3f velocity;
    Vector3f maxSpeed;
    Vector3f minSpeed;
    
    Vector3f acceleration;
    Vector3f slowDown;
    
    bool affectedByGravity;
    
    PhysicBody():
      affectedByGravity(false)
    {}
};
