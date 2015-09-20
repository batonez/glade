#pragma once

#include "../State.h"
#include "../GladeObject.h"

class Behavior
{
  public:
    virtual void act(State &world, GladeObject &self) = 0;
};
