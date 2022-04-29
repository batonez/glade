#pragma once

#include "../math/Vector.h"

class PhysicBody
{
  public:
    Glade::Vector3f velocity;
    Glade::Vector3f maxSpeed;
    Glade::Vector3f minSpeed;
    
    Glade::Vector3f acceleration;
    Glade::Vector3f slowDown;
    
    bool affectedByGravity;
    
    PhysicBody():
      affectedByGravity(false)
    {}
};
