package ru.hatchery.glade.physics;

import ru.hatchery.glade.GladeObject;
import ru.hatchery.glade.model.Transform;
import ru.hatchery.glade.model.Vector3f;

public class CollisionEvent
{
	public final GladeObject firstObject, secondObject;
	public final Vector3f overlap; 
	public final boolean firstIsAtTop;
	public final boolean firstIsAtLeft;
	
	public CollisionEvent(GladeObject kinematicObject, GladeObject staticObject, Vector3f overlap)
	{
		this.firstObject = kinematicObject;
		this.secondObject = staticObject;
		this.overlap = overlap;
		
		Transform kinematicColShapeTransform = kinematicObject.getTransform();
		Transform staticColShapeTransform = staticObject.getTransform();
		
		firstIsAtTop = kinematicColShapeTransform.getPosition().y < staticColShapeTransform.getPosition().y;
		firstIsAtLeft = kinematicColShapeTransform.getPosition().x < staticColShapeTransform.getPosition().x;
	}
}
