#pragma once

#include <vector>

class AiContainer
{
  public:
    bool paused;
    
	private:
    vector<GladeObject*> smartObjects;
	
	public:
    AiContainer(): paused(false) {}
    
    void process(State &world)
    {
      if (paused) {
        return;
      }
      
      vector<GladeObject>::iterator i = smartObjects.begin();
      
      while (i != smartObjects.end()) {
        if (i->getBehavior() != null && i->isBehaviorEnabled()) {
          i->getBehavior()->act(world, sceneObject);
        }
        
        ++i;
      }
    }
    
    void add(GladeObject* object)
    {
      smartObjects.add(object);
    }
    
    void clear()
    {
      smartObjects.clear();
      paused = false;
    }
}
