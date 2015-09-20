#pragma once

#include <string.h>
#include <set>
#include <map>

#include "log/log.h"
#include "math/Transform.h"
#include "render/Drawable.h"
#include "exception/GladeException.h"

class GladeObject {
public:
  typedef std::set<Drawable*>    Drawables;
  typedef Drawables::iterator    DrawablesI;

private:
  typedef std::map<Drawable*, bool> VisibilitySwitches;
  std::string name;
  
protected:
  bool enabled;
//	PhysicBody* physicBody;
//	CollisionShape* collisionShape;
//	Behavior* behavior;
  
  Drawables drawables;
	VisibilitySwitches visibilitySwitches;
//	Set<Sound> sounds;
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
		behaviorEnabled(true),
    name("Undefined")
	{
  }
  
	void setEnabled(bool enable)
  {
		enabled = enable;
	}
	
	bool isEnabled(void) const
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
	
	virtual void addDrawable(Drawable* view)
  {
		drawables.insert(view);
		visibilitySwitches[view] = true;
	}

  void addDrawables(Drawables &drawables)
  {
    for (DrawablesI di = drawables.begin(); di != drawables.end(); ++di) {
      addDrawable(*di);
    }
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
	void toggleView(Drawable& view, bool enable)
  {
  	VisibilitySwitches::iterator i = visibilitySwitches.find(&view);
    
    if (i != visibilitySwitches.end()) {
      i->second = enable;
    }
	}

	void toggleView(bool enable)
  {
		for (DrawablesI i = drawables.begin(); i != drawables.end(); i++) {
			toggleView(**i, enable);
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
	
	bool isViewEnabled(Drawable *view) const
  {
    VisibilitySwitches::const_iterator i = visibilitySwitches.find(view);
		
    if (i != visibilitySwitches.end()) {
      return i->second;
    }
    
    return false;
	}

	bool isCollisionShapeEnabled() const
  {
		return colShapeEnabled;
	}

	
	bool isBehaviorEnabled() const
  {
		return behaviorEnabled;
	}

	
	bool isPhysicBodyEnabled() const
  {
		return physicBodyEnabled;
	}
	
	/** This is for debugging purposes */
	const std::string* getName(void) const
  {
		return &name;
	}
	
	/** This is for debugging purposes */
	void setName(const std::string &name)
  {
    this->name = name;
	}
};