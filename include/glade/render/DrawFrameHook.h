#pragma once

class DrawFrameHook {
public:
  virtual void onBeforeDraw(void) = 0;
  virtual void onAfterDraw(void) = 0;
};
