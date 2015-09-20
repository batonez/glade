#pragma once

#include <vector>

#include "../GladeObject.h"
#include "CollisionEvent.h"
#include "CollisionEventListener.h"

class CollisionDetector
{
	private:
    vector<GladeObject*> dynamicCollidableObjects;
    vector<GladeObject*> staticCollidableObjects;
    vector<CollisionEventListener*> listeners;
    bool paused = false;
	
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
      
      vector<GladeObject*>::iterator di = dynamicCollidableObjects.begin();
      vector<GladeObject*>::iterator si;
      
      while (di != dynamicCollidableObjects.end()) {
        if (di->isCollisionShapeEnabled()) { 
          si = staticCollidableObjects.begin();
          
          while (si != staticCollidableObjects.end()) {
            detectAndDispatch(*di, *si);
            ++si;
          }
          
          vector<GladeObject>::iterator di2 = di;
          ++di2;
          
          while (di2 != dynamicCollidableObjects.end()) {
            detectAndDispatch(*di, *di2);
            ++di2;
          }
        }
        
        ++di;
      }
    }
        
    void add(GladeObject* object)
    {
      if (object->getCollisionShape() != null) {
        if (object->getPhysicBody() == null) {
          staticCollidableObjects.add(object);
        } else {
          dynamicCollidableObjects.add(object);
        }
      }
    }
    
    void addListener(CollisionEventListener* listener)
    {
      listeners.add(listener);
    }
    
    void removeListener(CollisionEventListener* listener)
    {
      listeners.remove(listener);
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
        Vector3f overlap = object1.getCollisionShape().testCollisionWith(
          object2.getCollisionShape(),
          object1.getTransform(),
          object2.getTransform()
        );
        
        if (overlap != null) {
          CollisionEvent collisionEvent(&object1, &object2, overlap);
          
          vector<CollisionEventListener>::iterator i = listeners.begin();
          
          while (i != listeners.end()) {
            if (listener.onCollision(collisionEvent)) {
              break;
            }
            
            ++i;
          }
        }
      }
    }
};
