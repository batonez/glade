#pragma once

#include "../math/Vector.h"

struct PhysicBody
{
  Vector3f velocity;
  Vector3f acceleration;
  bool affectedByGravity;
};
