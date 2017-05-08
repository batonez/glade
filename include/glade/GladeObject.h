#pragma once

#include <string.h>
#include <vector>
#include <map>

#include "math/Transform.h"

class Behavior;
class PhysicBody;
class CollisionShape;
class Drawable;

class GladeObject {
public:
  typedef std::vector<Drawable*>    Drawables;
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

  // Merges the meshes of another object into the mesh of
  // the first drawable of this object.
  // Returns the starting offset (in vertices) of the new mesh part
  // in the combined mesh's vertex buffer.
  int merge(GladeObject &object);
  virtual void setEnabled(bool enable);
  virtual bool isEnabled(void) const;
  virtual Behavior* getBehavior(void);
  virtual void setBehavior(Behavior *behavior);
  virtual void setTransform(Transform &transform);
  virtual Transform* getTransform(void);
  // FIXME should be unique_ptr<Drawable>
  virtual void addDrawable(Drawable* view);
  virtual void addDrawables(Drawables &drawables);
  virtual Drawables* getDrawables(void);

  /*
  void addSound(Sound sound);
  Collection<Sound> getSounds();
  */

  virtual void setPhysicBody(PhysicBody &physicBody);
  virtual PhysicBody* getPhysicBody();
  virtual CollisionShape* getCollisionShape(void);
  virtual void setCollisionShape(CollisionShape &collisionShape);
  virtual void toggleView(Drawable& view, bool enable);
  virtual void toggleView(bool enable);
  virtual void toggleCollisionShape(bool enable);
  virtual void toggleBehavior(bool enable);
  virtual void togglePhysicBody(bool enable);
  virtual bool isViewEnabled(Drawable *view) const;
  virtual bool isCollisionShapeEnabled() const;
  virtual bool isBehaviorEnabled() const;
  virtual bool isPhysicBodyEnabled() const;

  /** This is for debugging purposes */
  virtual const std::string* getName(void) const;
  virtual void setName(const std::string &name);
};