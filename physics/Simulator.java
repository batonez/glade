package ru.hatchery.glade.physics;

import java.util.HashSet;
import java.util.Iterator;
import java.util.Set;

import ru.hatchery.glade.GladeObject;
import ru.hatchery.glade.model.Vector3f;

public class Simulator
{
	public Vector3f gravity = new Vector3f(0, 0, 0);;
	private Set<GladeObject> dynamicObjects = new HashSet<GladeObject>();
	private boolean paused = false;

	public void clear()
	{
		dynamicObjects.clear();
		paused = false;
		gravity = new Vector3f(0, 0, 0);
	}
	
	public void stepSimulation(double deltaTime)
	{
		if (paused) {
			return;
		}
		
		Iterator<GladeObject> i = dynamicObjects.iterator();
		GladeObject dynamicObject;
		
		while (i.hasNext()) {
			dynamicObject = i.next();
			
			if (dynamicObject.getPhysicBody() != null && dynamicObject.isPhysicBodyEnabled()) {
				dynamicObject.getPhysicBody().stepSimulation(deltaTime, dynamicObject.getTransform());
				
				if (dynamicObject.getPhysicBody().getVelocity().y > 5f) { // constraining y velocity
					dynamicObject.getPhysicBody().getVelocity().y = 5f;
				}
				
				if (dynamicObject.getPhysicBody().isAffectedByGravity()) {
					dynamicObject.getPhysicBody().setAcceleration(gravity);
				}
			}
		}
	}
	
	public void add(GladeObject object)
	{
		if (object.getPhysicBody() != null) {
			dynamicObjects.add(object);
		}
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
