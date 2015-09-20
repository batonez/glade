#pragma once

#include <string.h>
#include <vector>
#include <map>

#include "log/log.h"
#include "math/Transform.h"
#include "render/Drawable.h"
#include "exception/GladeException.h"

class GladeObject {
protected:
	bool enabled;
//	PhysicBody* physicBody;
//	CollisionShape* collisionShape;
//	Behavior* behavior;
  
  typedef std::vector<Drawable*>    Drawables;
  typedef std::map<Drawable*, bool> VisibilitySwitches;
  
	Drawables drawables;
	VisibilitySwitches visibilitySwitches;
//	Set<Sound> sounds;
	char name[32];
	bool colShapeEnabled, physicBodyEnabled, behaviorEnabled;
	
private:
	Transform transform;
		
public:
	GladeObject(void):
		enabled(true),
//		physicBody(NULL),
//		collisionShape(NULL),
//		behavior(NULL),
		colShapeEnabled(true),
		physicBodyEnabled(true),
		behaviorEnabled(true)
	{}

	void setEnabled(bool enable)
  {
		enabled = enable;
	}
	
	bool isEnabled(void)
  {
		return enabled;
	}
/*	
	Behavior getBehavior(void) {
		return behavior;
	}

	void setBehavior(Behavior behavior) {
		this->behavior = behavior;
	}
*/
	
	void setTransform(Transform transform)
  {
		this->transform = transform;
	}
	
	
	Transform* getTransform(void)
  {
		return &transform;
	}
	
	void addDrawable(Drawable* view)
  {
		drawables.push_back(view);
		visibilitySwitches[view] = true;
	}

	Drawables* getDrawables(void)
  {
		return &drawables;
	}
	
	/*
	void addSound(Sound sound) {
		if (sound == null) {
			throw new GladeRuntimeException("Tried to add a Sound that is NULL to GladeObject");
		}
		
		sounds.add(sound);
	}*/
	
	/*
	Collection<Sound> getSounds() {
		return sounds;
	}
	
	
	void setPhysicBody(PhysicBody &physicBody) {
		this->physicBody = &physicBody;
	}
	
	PhysicBody* getPhysicBody(void) {
		return physicBody;
	}

	CollisionShape* getCollisionShape(void) {
		return collisionShape;
	}
	
	void setCollisionShape(CollisionShape &collisionShape) {
		this->collisionShape = &collisionShape;
	}
	*/
	void toggleView(Drawable* view, bool enable)
  {
  	VisibilitySwitches::iterator i = visibilitySwitches.find(view);
    
    if (i != visibilitySwitches.end()) {
      i->second = enable;
    }
	}

	void toggleView(bool enable)
  {
		for (Drawables::iterator i = drawables.begin(); i != drawables.end(); i++) {
			toggleView(*i, enable);
		}
	}
	
	void toggleCollisionShape(bool enable)
  {
		colShapeEnabled = enable;
	}
	
	void toggleBehavior(bool enable)
  {
		behaviorEnabled = enable;
	}

	void togglePhysicBody(bool enable)
  {
		physicBodyEnabled = enable;
	}
	
	bool isViewEnabled(Drawable* view)
  {
    VisibilitySwitches::iterator i = visibilitySwitches.find(view);
		
    if (i != visibilitySwitches.end()) {
      return i->second;
    }
    
    return false;
	}

	bool isCollisionShapeEnabled()
  {
		return colShapeEnabled;
	}

	
	bool isBehaviorEnabled()
  {
		return behaviorEnabled;
	}

	
	bool isPhysicBodyEnabled()
  {
		return physicBodyEnabled;
	}
	
	/** This is for debugging purposes */
	char* getName(void)
  {
		return name;
	}
	
	/** This is for debugging purposes */
	void setName(const char* name)
  {
		if (strlen(name) < 32) {
			strcpy(this->name, name);
		} else {
      throw GladeException("GladeObject::setName() name is too long");
    }
	}
};