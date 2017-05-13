#pragma once

class VirtualController
{
  public:
    virtual void init() = 0;
    virtual bool buttonPress(int controlId, int terminalId) = 0;
    virtual bool buttonRelease(int controlId, int terminalId) = 0;
    virtual bool pointerDown(float axisX, float axisY, float axisZ, int controlId, int terminalId) = 0;
    virtual bool pointerUp(float axisX, float axisY, float axisZ, int controlId, int terminalId) = 0;
    virtual bool pointerMove(float axisX, float axisY, float axisZ, int controlId, int terminalId) = 0;
};
