#pragma once

#include <string.h>
#include <set>
#include <map>

#include "math/Transform.h"

class Behavior;
class PhysicBody;
class CollisionShape;
class Drawable;

class GladeObject {
public:
  typedef std::set<Drawable*>    Drawables;
  typedef Drawables::iterator    DrawablesI;

  class Comparator
  {
    public:
      virtual int compare(GladeObject &first, GladeObject &second) = 0;
  };

protected:
  bool enabled;
  PhysicBody* physicBody;
  CollisionShape* collisionShape;
  Behavior* behavior;
  Drawables drawables;
  std::map<Drawable*, bool> visibilitySwitches;
//Set<Sound> sounds;
  bool colShapeEnabled, physicBodyEnabled, behaviorEnabled;

private:
  std::string name;
  Transform transform;

public:
  GladeObject(void);
  void setEnabled(bool enable);
  bool isEnabled(void) const;
  Behavior* getBehavior(void);
  void setBehavior(Behavior *behavior);
  void setTransform(Transform &transform);
  Transform* getTransform(void);
  // FIXME should be unique_ptr<Drawable>
  virtual void addDrawable(Drawable* view);
  void addDrawables(Drawables &drawables);
  Drawables* getDrawables(void);

  /*
  void addSound(Sound sound);
  Collection<Sound> getSounds();
  */

  void setPhysicBody(PhysicBody &physicBody);
  PhysicBody* getPhysicBody(void);
  CollisionShape* getCollisionShape(void);
  void setCollisionShape(CollisionShape &collisionShape);
  void toggleView(Drawable& view, bool enable);
  void toggleView(bool enable);
  void toggleCollisionShape(bool enable);
  void toggleBehavior(bool enable);
  void togglePhysicBody(bool enable);
  bool isViewEnabled(Drawable *view) const;
  bool isCollisionShapeEnabled() const;
  bool isBehaviorEnabled() const;
  bool isPhysicBodyEnabled() const;

  /** This is for debugging purposes */
  const std::string* getName(void) const;
  void setName(const std::string &name);
};