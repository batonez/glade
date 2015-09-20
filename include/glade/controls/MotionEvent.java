package ru.hatchery.glade.controls;

import ru.hatchery.glade.Timer;

public class MotionEvent
{
	public final float x, y, z;
	public final Integer controlId, terminalId;
	public final double createdAt;
	
	public MotionEvent(float x, float y, float z, Integer controlId, Integer terminalId)
	{
		this.createdAt = Timer.getAbsoluteTimestamp();
		this.x = x;
		this.y = y;
		this.z = z;
		this.controlId = controlId;
		this.terminalId = terminalId;
	}
}
