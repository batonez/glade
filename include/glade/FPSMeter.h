#pragma once

class FPSMeter
{
	private:
    float fps;
	
  public:
    /**
     * 
     * @param deltaTime Time in milliseconds that was taken to draw 1 frame
     */
    void updateFPS(double deltaTime)
    {
      if (deltaTime <= 0) {
        fps = -1;
      } else {	
        fps = (float) (1000.0 / deltaTime);
      }
    }

    float getFPS()
    {
      return fps;
    }
};
