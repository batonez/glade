#pragma once

#include <set>
#include "glade/GladeObject.h"
#include "PhysicBody.h"

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
        PhysicBody *body = (*i)->getPhysicBody();
        Transform *transform = (*i)->getTransform();

        if (body != NULL && (*i)->isPhysicBodyEnabled()) {
          body->velocity.x += deltaTime * body->acceleration.x;
          body->velocity.y += deltaTime * body->acceleration.y;
          body->velocity.z += deltaTime * body->acceleration.z;

          transform->getPosition()->x += deltaTime * body->velocity.x;
          transform->getPosition()->y += deltaTime * body->velocity.y;
          transform->getPosition()->z += deltaTime * body->velocity.z;

          // constraining y velocity
          if (body->velocity.y > 5.0f) {
            body->velocity.y = 5.0f;
          }

          if (body->affectedByGravity) {
            body->acceleration = gravity;
          }
        }
      }
    }

    void add(GladeObject* object)
    {
      if (object->getPhysicBody() != NULL) {
        dynamicObjects.insert(object);
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
