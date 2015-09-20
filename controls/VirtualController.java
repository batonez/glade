package ru.hatchery.glade.controls;

public interface VirtualController
{
	void init();
	boolean buttonPress(Integer controlId, Integer terminalId);
	boolean buttonRelease(Integer controlId, Integer terminalId);
	boolean pointerDown(float axisX, float axisY, float axisZ, Integer controlId, Integer terminalId);
	boolean pointerUp(float axisX, float axisY, float axisZ, Integer controlId, Integer terminalId);
	boolean pointerMove(float axisX, float axisY, float axisZ, Integer controlId, Integer terminalId);
}
