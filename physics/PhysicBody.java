package ru.hatchery.glade.physics;

import ru.hatchery.glade.model.Transform;
import ru.hatchery.glade.model.Vector3f;

public class PhysicBody
{
	protected Vector3f velocity = new Vector3f();
	protected Vector3f acceleration = new Vector3f();
	protected boolean affectedByGravity = true;
	
	public void stepSimulation(double deltaTime, Transform transform)
	{
		velocity.x += deltaTime * acceleration.x;		
		velocity.y += deltaTime * acceleration.y;
		velocity.z += deltaTime * acceleration.z;
		
		transform.getPosition().x += deltaTime * velocity.x;
		transform.getPosition().y += deltaTime * velocity.y;
		transform.getPosition().z += deltaTime * velocity.z;
	}

	public void setVelocity(Vector3f velocity)
	{
		this.velocity.x = velocity.x;
		this.velocity.y = velocity.y;
		this.velocity.z = velocity.z;
	}	
	
	public void setAcceleration(Vector3f acceleration)
	{
		this.acceleration.x = acceleration.x;
		this.acceleration.y = acceleration.y;
		this.acceleration.z = acceleration.z;
	}
	
	public Vector3f getAcceleration()
	{
		return acceleration;
	}
	
	public Vector3f getVelocity()
	{
		return velocity;
	}
	
	public boolean isAffectedByGravity()
	{
		return affectedByGravity;
	}
	
	public void setAffectedByGravity(boolean affectedByGravity)
	{
		this.affectedByGravity = affectedByGravity;
	}
}
