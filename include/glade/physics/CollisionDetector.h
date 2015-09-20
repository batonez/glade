#pragma once

class GladeObject;
class CollisionEventListener;

class CollisionDetector
{
  private:
    std::vector<GladeObject*> dynamicCollidableObjects;
    std::vector<GladeObject*> staticCollidableObjects;
    std::vector<CollisionEventListener*> listeners;
    bool paused;

  public:
    CollisionDetector(): paused(false) {}
    void clear();
    void clearObjects();
    void clearListeners();
    void detectCollisions(double deltaTime);
    void add(GladeObject* object);
    void remove(GladeObject* object);
    void addListener(CollisionEventListener* listener);
    void removeListener(CollisionEventListener* listener);

    void setPaused(bool paused)
    {
      this->paused = paused;
    }

    bool isPaused()
    {
      return paused;
    }

  private:
    void detectAndDispatch(GladeObject &object1, GladeObject &object2);
};
