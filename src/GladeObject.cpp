#include <glade/GladeObject.h>
#include <glade/math/Matrix.h>
#include <glade/render/definitions.h>
#include <glade/render/Drawable.h>
#include <glade/render/meshes/Mesh.h>
#include <glade/debug/log.h>
#include <glade/exception/GladeException.h>

GladeObject::GladeObject(void):
  enabled(true),
  physicBody(NULL),
  collisionShape(NULL),
  behavior(NULL),
  colShapeEnabled(true),
  physicBodyEnabled(true),
  behaviorEnabled(true),
  name("Undefined")
{
}

int GladeObject::merge(GladeObject &object)
{
  if (drawables.begin() == drawables.end()) {
    log("Warning: failed to merge object meshes, because destination object doesn't have any drawable");
    return 0;
  }

  Drawable * firstDrawableOfSelf = *drawables.begin();
  Drawables* otherDrawables = object.getDrawables();  

  int numberOfVerticesBeforeMergingTheObject = (*drawables.begin())->getMesh()->vertices.size() / GLADE_VERTEX_STRIDE_FLOATS;
  int numberOfVerticesBeforeMergingTheDrawable = numberOfVerticesBeforeMergingTheObject;

  for (DrawablesI di = otherDrawables->begin(); di != otherDrawables->end(); ++di) {
    Glade::Mesh otherMeshCopy = *(*di)->getMesh();
    float worldMatrix[16], drawableMatrix[16];
    object.getTransform()->getMatrix(worldMatrix);
    (*di)->getTransform()->getMatrix(drawableMatrix);
    float initialMeshVertexPos[4], worldVertexPos[4], drawableVertexPos[4];

    for (int i = 0; i < otherMeshCopy.vertices.size(); i += GLADE_VERTEX_STRIDE_FLOATS) {
      initialMeshVertexPos[0] = otherMeshCopy.vertices[i + 0];
      initialMeshVertexPos[1] = otherMeshCopy.vertices[i + 1];
      initialMeshVertexPos[2] = otherMeshCopy.vertices[i + 2];
      initialMeshVertexPos[3] = 1.0f;

      Matrix::multiplyMV(worldVertexPos, worldMatrix, initialMeshVertexPos);
      Matrix::multiplyMV(drawableVertexPos, drawableMatrix, worldVertexPos);

      otherMeshCopy.vertices[i + 0] = drawableVertexPos[0];
      otherMeshCopy.vertices[i + 1] = drawableVertexPos[1];
      otherMeshCopy.vertices[i + 2] = drawableVertexPos[2];
    }

    for (int i = 0; i < otherMeshCopy.indices.size(); ++i) {
      otherMeshCopy.indices[i] += numberOfVerticesBeforeMergingTheDrawable;
    }

    firstDrawableOfSelf->getMesh()->vertices.insert(firstDrawableOfSelf->getMesh()->vertices.end(), otherMeshCopy.vertices.begin(), otherMeshCopy.vertices.end());
    firstDrawableOfSelf->getMesh()->indices.insert(firstDrawableOfSelf->getMesh()->indices.end(), otherMeshCopy.indices.begin(), otherMeshCopy.indices.end());

    numberOfVerticesBeforeMergingTheDrawable += otherMeshCopy.vertices.size();
  }

  return numberOfVerticesBeforeMergingTheObject;
}

void GladeObject::setEnabled(bool enable)
{
  enabled = enable;
}

bool GladeObject::isEnabled(void) const
{
  return enabled;
}

Behavior* GladeObject::getBehavior(void) {
  return behavior;
}

void GladeObject::setBehavior(Behavior *behavior) {
  this->behavior = behavior;
}

void GladeObject::setTransform(Transform &transform)
{
  this->transform = transform;
}

Transform* GladeObject::getTransform(void)
{
  return &transform;
}

void GladeObject::addDrawable(Drawable* view)
{
  drawables.push_back(view);
  visibilitySwitches[view] = true;
}

void GladeObject::addDrawables(Drawables &drawables)
{
  for (DrawablesI di = drawables.begin(); di != drawables.end(); ++di) {
    addDrawable(*di);
  }
}

GladeObject::Drawables* GladeObject::getDrawables(void)
{
  return &drawables;
}

/*
void addSound(Sound sound) {
  if (sound == null) {
    throw new GladeRuntimeException("Tried to add a Sound that is NULL to GladeObject");
  }

  sounds.add(sound);
}

Collection<Sound> getSounds() {
  return sounds;
}
*/

void GladeObject::setPhysicBody(PhysicBody &physicBody)
{
  this->physicBody = &physicBody;
}

PhysicBody* GladeObject::getPhysicBody()
{
  return physicBody;
}

CollisionShape* GladeObject::getCollisionShape(void) {
  return collisionShape;
}

void GladeObject::setCollisionShape(CollisionShape &collisionShape) {
  this->collisionShape = &collisionShape;
}

void GladeObject::toggleView(Drawable& view, bool enable)
{
  std::map<Drawable*,bool>::iterator i = visibilitySwitches.find(&view);

  if (i != visibilitySwitches.end()) {
    i->second = enable;
  }
}

void GladeObject::toggleView(bool enable)
{
  for (DrawablesI i = drawables.begin(); i != drawables.end(); i++) {
    toggleView(**i, enable);
  }
}

void GladeObject::toggleCollisionShape(bool enable)
{
  colShapeEnabled = enable;
}

void GladeObject::toggleBehavior(bool enable)
{
  behaviorEnabled = enable;
}

void GladeObject::togglePhysicBody(bool enable)
{
  physicBodyEnabled = enable;
}

bool GladeObject::isViewEnabled(Drawable *view) const
{
  std::map<Drawable*,bool>::const_iterator i = visibilitySwitches.find(view);

  if (i != visibilitySwitches.end()) {
    return i->second;
  }

  return false;
}

bool GladeObject::isCollisionShapeEnabled() const
{
  return colShapeEnabled;
}


bool GladeObject::isBehaviorEnabled() const
{
  return behaviorEnabled;
}


bool GladeObject::isPhysicBodyEnabled() const
{
  return physicBodyEnabled;
}

/** This is for debugging purposes */
const std::string* GladeObject::getName(void) const
{
  return &name;
}

/** This is for debugging purposes */
void GladeObject::setName(const std::string &name)
{
  this->name = name;
}