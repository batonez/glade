#include <vector>
#include <algorithm>

#include <glade/debug/log.h>
#include <glade/physics/CollisionDetector.h>
#include <glade/physics/CollisionShape.h>
#include <glade/physics/CollisionEvent.h>
#include <glade/physics/CollisionEventListener.h>
#include <glade/GladeObject.h>

void CollisionDetector::clear()
{
  clearObjects();
  clearListeners();
  paused = false;
}

void CollisionDetector::clearObjects()
{
  staticCollidableObjects.clear();
  dynamicCollidableObjects.clear();
}

void CollisionDetector::clearListeners()
{
  listeners.clear();
}

void CollisionDetector::detectCollisions(long deltaTime)
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

void CollisionDetector::add(GladeObject* object)
{
  if (object->getCollisionShape() != NULL) {
    if (object->getPhysicBody() == NULL) {
      staticCollidableObjects.push_back(object);
    } else {
      dynamicCollidableObjects.push_back(object);
    }
  }
}

void CollisionDetector::remove(GladeObject* object)
{
  std::vector<GladeObject*>::iterator oi =
    std::find(dynamicCollidableObjects.begin(), dynamicCollidableObjects.end(), object);
  
  if (oi == dynamicCollidableObjects.end()) {
    oi = std::find(staticCollidableObjects.begin(), staticCollidableObjects.end(), object);
    
    if (oi == staticCollidableObjects.end()) {
      log("Warning: could not remove object becasuse it's not in the simulator");
      return;
    }
    
    staticCollidableObjects.erase(oi);
    return;
  }
  
  dynamicCollidableObjects.erase(oi);
}

void CollisionDetector::addListener(CollisionEventListener* listener)
{
  listeners.push_back(listener);
}

void CollisionDetector::removeListener(CollisionEventListener* listener)
{
  std::vector<CollisionEventListener*>::iterator i = std::find(listeners.begin(), listeners.end(), listener);

  if (i != listeners.end()) {
    listeners.erase(i);
  }
}

void CollisionDetector::detectAndDispatch(GladeObject &object1, GladeObject &object2)
{
  Vector3f overlap;
  
  if (object2.isCollisionShapeEnabled()) {
     bool collided = object1.getCollisionShape()->testCollisionWith(
      object2.getCollisionShape(),
      object1.getTransform(),
      object2.getTransform(),
      overlap
    );
    
    if (collided) {
      CollisionEvent collisionEvent(&object1, &object2, overlap);
    
      log("COLLISION DETECTED: overlap is (%3.3f, %3.3f, %3.3f)", overlap.x, overlap.y, overlap.z);

      std::vector<CollisionEventListener*>::iterator i = listeners.begin();

      while (i != listeners.end()) {
        if ((*i)->onCollision(collisionEvent)) {
          break;
        }

        ++i;
      }
    }
  }
}