#pragma once

#include "glade/Scene.h"
#include "glade/GladeObject.h"

class Behavior
{
  public:
    virtual void act(Scene &world, GladeObject &self) = 0;
};
