#pragma once

#include <ctime>

class Timer {
private:
  bool paused;
  clock_t baseClockstamp;
public:
  Timer(void):
    paused(false),
    baseClockstamp(0L)
  {}
  
  /**
   * Returns number of milliseconds have passed since the last call to the function reset() or 0 if the timer is paused or reset() was never called
   */
  float getDeltaTime(void) {
    if (baseClockstamp == 0L) { // reset was never called
      return 0;
    }
    
    clock_t currentClockstamp = clock();
    clock_t deltaClock = currentClockstamp - baseClockstamp;
        
    return paused ? 0.0f: ((float)(1000 * deltaClock) / (float) CLOCKS_PER_SEC);
  }
  
  void reset(void) {
    baseClockstamp = clock();
  }
  
  void pause(void) {
    paused = true;
  }
  
  void unpause(void) {
    paused = false;
  }
};
