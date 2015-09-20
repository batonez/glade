#pragma once

#include <set>
#include "../GladeObject.h"

class Simulator
{
	public:
    Vector3f gravity;
	private:
    set<GladeObject*> dynamicObjects;
    bool paused;

	public:
    Simulator(): paused(false) {}
    
    void clear()
    {
      dynamicObjects.clear();
      paused = false;
      gravity.set(0, 0, 0);
    }
    
    void stepSimulation(double deltaTime)
    {
      if (paused) {
        return;
      }
      
      set<GladeObject*>::iterator i = dynamicObjects.begin();
      
      while (i != dynamicObjects.end()) {
        if (i->getPhysicBody() != null && i->isPhysicBodyEnabled()) {
          i->getPhysicBody()->stepSimulation(deltaTime, i->getTransform());
          
          if (i->getPhysicBody()->getVelocity()->y > 5f) { // constraining y velocity
            i->getPhysicBody()->getVelocity()->y = 5f;
          }
          
          if (i->getPhysicBody()->isAffectedByGravity()) {
            i->getPhysicBody()->setAcceleration(gravity);
          }
        }
      }
    }
    
    void add(GladeObject* object)
    {
      if (object->getPhysicBody() != null) {
        dynamicObjects.add(object);
      }
    }
    
    void setPaused(bool paused)
    {
      this->paused = paused;
    }
    
    bool isPaused()
    {
      return paused;
    }
};
