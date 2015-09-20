#include <glade/physics/Simulator.h>
#include <glade/physics/PhysicBody.h>
#include <glade/GladeObject.h>

void Simulator::clear()
{
  dynamicObjects.clear();
  paused = false;
  gravity.set(0, 0, 0);
}

void Simulator::add(GladeObject* object)
{
  if (object->getPhysicBody() != NULL) {
    dynamicObjects.insert(object);
  }
}

void Simulator::stepSimulation(double deltaTime)
{
  if (paused) {
    return;
  }

  std::set<GladeObject*>::iterator i = dynamicObjects.begin();

  while (i != dynamicObjects.end()) {
    PhysicBody *body = (*i)->getPhysicBody();
    Transform *transform = (*i)->getTransform();

    if (body != NULL && (*i)->isPhysicBodyEnabled()) {
      body->velocity.x += deltaTime * body->acceleration.x;
      body->velocity.y += deltaTime * body->acceleration.y;
      body->velocity.z += deltaTime * body->acceleration.z;

      transform->getPosition()->x += deltaTime * body->velocity.x;
      transform->getPosition()->y += deltaTime * body->velocity.y;
      transform->getPosition()->z += deltaTime * body->velocity.z;

      // constraining y velocity
      if (body->velocity.y > 5.0f) {
        body->velocity.y = 5.0f;
      }

      if (body->affectedByGravity) {
        body->acceleration = gravity;
      }
    }
  }
}