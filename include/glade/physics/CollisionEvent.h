#pragma once

#include "../GladeObject.h"
#include "../math/Vector.h"

class CollisionEvent
{
  public:
    GladeObject *firstObject, *secondObject;
    Vector3f overlap;
    bool firstIsAtTop;
    bool firstIsAtLeft;

  CollisionEvent(GladeObject* kinematicObject, GladeObject* staticObject, Vector3f &overlap):
    firstObject(kinematicObject),
    secondObject(staticObject),
    overlap(overlap)
  {
    Transform *kinematicColShapeTransform = kinematicObject->getTransform();
    Transform *staticColShapeTransform = staticObject->getTransform();

    firstIsAtTop = kinematicColShapeTransform->getPosition()->y < staticColShapeTransform->getPosition()->y;
    firstIsAtLeft = kinematicColShapeTransform->getPosition()->x < staticColShapeTransform->getPosition()->x;
  }
};
