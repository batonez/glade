#pragma once

#include <vector>
#include <algorithm>

#include "glade/GladeObject.h"
#include "CollisionShape.h"
#include "CollisionEvent.h"
#include "CollisionEventListener.h"

class CollisionDetector
{
  private:
    std::vector<GladeObject*> dynamicCollidableObjects;
    std::vector<GladeObject*> staticCollidableObjects;
    std::vector<CollisionEventListener*> listeners;
    bool paused;

  public:
    CollisionDetector(): paused(false) {}

    void clear()
    {
      clearObjects();
      clearListeners();
      paused = false;
    }

    void clearObjects()
    {
      staticCollidableObjects.clear();
      dynamicCollidableObjects.clear();
    }

    void clearListeners()
    {
      listeners.clear();
    }

    void detectCollisions(double deltaTime)
    {
      if (paused) {
        return;
      }

      std::vector<GladeObject*>::iterator di = dynamicCollidableObjects.begin();
      std::vector<GladeObject*>::iterator si;

      while (di != dynamicCollidableObjects.end()) {
        if ((*di)->isCollisionShapeEnabled()) {
          si = staticCollidableObjects.begin();

          while (si != staticCollidableObjects.end()) {
            detectAndDispatch(**di, **si);
            ++si;
          }

          std::vector<GladeObject*>::iterator di2 = di;
          ++di2;

          while (di2 != dynamicCollidableObjects.end()) {
            detectAndDispatch(**di, **di2);
            ++di2;
          }
        }

        ++di;
      }
    }

    void add(GladeObject* object)
    {
      if (object->getCollisionShape() != NULL) {
        if (object->getPhysicBody() == NULL) {
          staticCollidableObjects.push_back(object);
        } else {
          dynamicCollidableObjects.push_back(object);
        }
      }
    }

    void addListener(CollisionEventListener* listener)
    {
      listeners.push_back(listener);
    }

    void removeListener(CollisionEventListener* listener)
    {
      std::vector<CollisionEventListener*>::iterator i = std::find(listeners.begin(), listeners.end(), listener);

      if (i != listeners.end()) {
        listeners.erase(i);
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

  private:
    void detectAndDispatch(GladeObject &object1, GladeObject &object2)
    {
      if (object2.isCollisionShapeEnabled()) {
        Vector3f overlap = object1.getCollisionShape()->testCollisionWith(
          object2.getCollisionShape(),
          object1.getTransform(),
          object2.getTransform()
        );

        CollisionEvent collisionEvent(&object1, &object2, overlap);

        std::vector<CollisionEventListener*>::iterator i = listeners.begin();

        while (i != listeners.end()) {
          if ((*i)->onCollision(collisionEvent)) {
            break;
          }

          ++i;
        }
      }
    }
};
