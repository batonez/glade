package ru.hatchery.glade.physics;

import java.util.ArrayList;
import java.util.Iterator;

import ru.hatchery.glade.GladeObject;
import ru.hatchery.glade.model.Vector3f;

/**
 * 
 * Singleton
 */
public class CollisionDetector
{
	private ArrayList<GladeObject> dynamicCollidableObjects = new ArrayList<GladeObject>();
	private ArrayList<GladeObject> staticCollidableObjects = new ArrayList<GladeObject>();
	private ArrayList<CollisionEventListener> listeners = new ArrayList<CollisionEventListener>();
	private boolean paused = false;
	
	public void clear()
	{
		clearObjects();
		clearListeners();
		paused = false;
	}
	
	public void clearObjects()
	{
		staticCollidableObjects.clear();
		dynamicCollidableObjects.clear();
	}
	
	public void clearListeners()
	{
		listeners.clear();
	}
	
	public void detectCollisions(double deltaTime)
	{
		if (paused) {
			return;
		}
		
		Iterator<GladeObject> di = dynamicCollidableObjects.iterator();
		Iterator<GladeObject> si;
		GladeObject testedObject;
		
		while (di.hasNext()) {
			testedObject = di.next();
		
			if (testedObject.isCollisionShapeEnabled()) { 
				si = staticCollidableObjects.iterator();
				
				while (si.hasNext()) {
					detectAndDispatch(testedObject, si.next());
				}
				
				Iterator<GladeObject> di2 = dynamicCollidableObjects.iterator();
				
				while (di2.hasNext()) { // TODO Неправильно. Не нужно проверять те объекты, которые уже были проверены итератором di. Наверное для этого лучше использовать LinkedList
					detectAndDispatch(testedObject, di2.next());
				}
			}
		}
	}
	
	private void detectAndDispatch(GladeObject object1, GladeObject object2)
	{
		CollisionEvent collisionEvent;
		Vector3f overlap;

		if (object2.isCollisionShapeEnabled()) {
			overlap = object1.getCollisionShape().testCollisionWith(
				object2.getCollisionShape(),
				object1.getTransform(),
				object2.getTransform()
			);
			
			if (overlap != null) {
				collisionEvent = new CollisionEvent(object1, object2, overlap);
				
				for (CollisionEventListener listener: listeners) {
					if (listener.onCollision(collisionEvent)) {
						break;
					}
				}
			}
		}
	}
	
	public void add(GladeObject object)
	{
		if (object.getCollisionShape() != null) {
			if (object.getPhysicBody() == null) {
				staticCollidableObjects.add(object);
			} else {
				dynamicCollidableObjects.add(object);
			}
		}
	}
	
	public void addListener(CollisionEventListener listener)
	{
		
		listeners.add(listener);
	}
	
	public void removeListener(CollisionEventListener listener)
	{
		
		listeners.remove(listener);
	}
	
	public void setPaused(boolean paused)
	{
		this.paused = paused;
	}
	
	public boolean isPaused()
	{
		return paused;
	}
}
