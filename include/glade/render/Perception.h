#pragma once

#include <glade/render/Drawable.h>

class Perception: public Drawable
{
  private:
    Vector3f baseLightDirection;
    Vector3f baseLookVector;

    Vector3f halfplane;
    Vector3f lightDirection;
    Vector3f lookVector;

  public:
    Perception();
    void adjust();
    virtual ~Perception() {}
};
