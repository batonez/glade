#pragma once

#include "../GladeObject.h"
#include "../math/Vector.h"

class CollisionEvent const
{
	public:
    GladeObject* firstObject, secondObject;
    Vector3f overlap; 
    bool firstIsAtTop;
    bool firstIsAtLeft;
	
	CollisionEvent(GladeObject* kinematicObject, GladeObject* staticObject, Vector3f &overlap)
	{
		this->firstObject = kinematicObject;
		this->secondObject = staticObject;
		this->overlap = overlap;
		
		Transform kinematicColShapeTransform = kinematicObject->getTransform();
		Transform staticColShapeTransform = staticObject->getTransform();
		
		firstIsAtTop = kinematicColShapeTransform.getPosition().y < staticColShapeTransform.getPosition().y;
		firstIsAtLeft = kinematicColShapeTransform.getPosition().x < staticColShapeTransform.getPosition().x;
	}
};
