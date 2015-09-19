package ru.hatchery.glade.physics;

import ru.hatchery.glade.model.Vector3f;

public class RectangleCollisionResolver implements CollisionEventListener
{
	private byte direction;
	private byte axis;
	private float distance;
	private static final byte HORIZONTAL = 0;
	private static final byte VERTICAL = 1;
	
	@Override
	public boolean onCollision(CollisionEvent collisionEvent)
	{
		// dis is not good, gotta do a proper method with argument type declarations (or at least check instanceof)
		RectangleCollisionShape firstColShape = (RectangleCollisionShape) collisionEvent.firstObject.getCollisionShape();
		RectangleCollisionShape secondColShape = (RectangleCollisionShape) collisionEvent.secondObject.getCollisionShape();
				
		Vector3f firstCorrection = new Vector3f();
		Vector3f secondCorrection = new Vector3f();
		
		determinePushDirections(collisionEvent);
		
		if (axis == HORIZONTAL) {
			if (firstColShape.getType() == CollisionShape.STATIC) {
				secondCorrection.x = distance * direction;
				
				/*if (secondObject.getPhysicBody().getVelocity().x < 0) {
					secondObject.getPhysicBody().getVelocity().x = 0;
				}*/
			} else if (secondColShape.getType() == CollisionShape.STATIC) {
				firstCorrection.x = - distance * direction;
				
				/*if (firstObject.getPhysicBody().getVelocity().x > 0) {
					firstObject.getPhysicBody().getVelocity().x = 0;
				}*/
			} else {
				firstCorrection.x = - distance * direction / 2;
				secondCorrection.x =  distance * direction / 2;
			}
		} else if (axis == VERTICAL) {
			if (firstColShape.getType() == CollisionShape.STATIC) {
				secondCorrection.y = distance * direction;
				
				if (collisionEvent.secondObject.getPhysicBody().getVelocity().y < 0) {
					collisionEvent.secondObject.getPhysicBody().getVelocity().y = 0;
				}
			} else if (secondColShape.getType() == CollisionShape.STATIC) {
				firstCorrection.y = - distance * direction;
	
				if (collisionEvent.firstObject.getPhysicBody().getVelocity().y > 0) {
					collisionEvent.firstObject.getPhysicBody().getVelocity().y = 0;
				}
				
			} else {
				firstCorrection.y = - distance * direction / 2;
				secondCorrection.y =  distance * direction / 2;
			}
		}
		
		collisionEvent.firstObject.getTransform().getPosition().addAndSave(firstCorrection);
		collisionEvent.secondObject.getTransform().getPosition().addAndSave(secondCorrection);
		
		return false;
	}
	
	private void determinePushDirections(CollisionEvent collisionEvent)
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
}
