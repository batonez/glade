#pragma once

#include <glade/render/Drawable.h>

class Perception: public Drawable
{
  private:
    Vector3f baseLightDirection;
    Vector3f lightDirection;

  public:
    Perception();
    void adjust();
    virtual ~Perception() {}
};
