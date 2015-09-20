package ru.hatchery.glade.controls;

public class VoidController implements VirtualController
{
  @Override
  public void init()
  {
    
  }
  
  @Override
  public boolean buttonPress(Integer controlId, Integer terminalId)
  {
    return true;
  }

  @Override
  public boolean buttonRelease(Integer controlId, Integer terminalId)
  {
    return true;
  }

  @Override
  public boolean pointerMove(float axisX, float axisY, float axisZ, Integer controlId, Integer terminalId)
  {
    return true;
  }

  @Override
  public boolean pointerDown(float axisX, float axisY, float axisZ, Integer controlId, Integer terminalId)
  {
    return true;
  }

  @Override
  public boolean pointerUp(float axisX, float axisY, float axisZ, Integer controlId, Integer terminalId)
  {
    return true;
  }
}
