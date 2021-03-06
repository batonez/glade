#pragma once

#include "CollisionShape.h"
#include "RectangleCollisionShape.h"
#include "CollisionEvent.h"
#include "CollisionEventListener.h"
#include "PhysicBody.h"
#include <glade/math/Vector.h>
#include <glade/debug/log.h>

class RectangleCollisionResolver: public CollisionEventListener
{
  private:
    static const char HORIZONTAL = 0, VERTICAL = 1;
    char direction, axis;
    float distance;
  
  public:
    bool onCollision(CollisionEvent &collisionEvent)
    {
      // dis is not good, gotta do a proper method with argument type declarations
      RectangleCollisionShape* firstColShape = dynamic_cast<RectangleCollisionShape*>(collisionEvent.firstObject->getCollisionShape());
      RectangleCollisionShape* secondColShape = dynamic_cast<RectangleCollisionShape*>(collisionEvent.secondObject->getCollisionShape());
          
      Glade::Vector3f firstCorrection;
      Glade::Vector3f secondCorrection;
      
      determinePushDirections(collisionEvent);
      
      if (axis == HORIZONTAL) {
        if (firstColShape->getType() == CollisionShape::STATIC) {
          secondCorrection.x = distance * direction;
          collisionEvent.secondObject->getPhysicBody()->velocity.x = 0;
        } else if (secondColShape->getType() == CollisionShape::STATIC) {
          firstCorrection.x = - distance * direction;
          collisionEvent.firstObject->getPhysicBody()->velocity.x = 0;
        } else {
          firstCorrection.x = - distance * direction / 2;
          secondCorrection.x =  distance * direction / 2;
        }
      } else if (axis == VERTICAL) {
        if (firstColShape->getType() == CollisionShape::STATIC) {
          secondCorrection.y = distance * direction;
          collisionEvent.secondObject->getPhysicBody()->velocity.y = 0;
        } else if (secondColShape->getType() == CollisionShape::STATIC) {
          firstCorrection.y = - distance * direction;
          collisionEvent.firstObject->getPhysicBody()->velocity.y = 0;
        } else {
          firstCorrection.y = - distance * direction / 2;
          secondCorrection.y =  distance * direction / 2;
        }
      }
      
      collisionEvent.firstObject->getTransform()->getPosition()->add(firstCorrection);
      collisionEvent.secondObject->getTransform()->getPosition()->add(secondCorrection);
      
      return false;
    }
  
  private:
    void determinePushDirections(CollisionEvent &collisionEvent)
    {
      if (collisionEvent.overlap.x < collisionEvent.overlap.y) {
        distance = collisionEvent.overlap.x;
        axis = HORIZONTAL;
        
        if (collisionEvent.firstIsAtLeft) {
          direction = 1;
        } else {
          direction = -1;
        }
      } else {
        distance = collisionEvent.overlap.y;
        axis = VERTICAL;
        
        if (collisionEvent.firstIsAtTop) {
          direction = 1;
        } else {
          direction = -1;
        }
      }
    }
};
