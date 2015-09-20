#include <vector>
#include <algorithm>

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

void CollisionDetector::detectCollisions(double deltaTime)
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