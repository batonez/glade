#pragma once

#include <set>
#include <glade/math/Vector.h>

class GladeObject;
class PhysicBody;

class Simulator
{
  public:
    Vector3f gravity;
  private:
    std::set<GladeObject*> dynamicObjects;
    bool paused;

  public:
    Simulator(): paused(false) {}
    void clear();
    void stepSimulation(double deltaTime);
    void add(GladeObject* object);
    void remove(GladeObject* object);
    
    void setPaused(bool paused)
    {
      this->paused = paused;
    }

    bool isPaused()
    {
      return paused;
    }
};
