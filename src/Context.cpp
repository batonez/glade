#include <glade/Context.h>
#include <glade/render/GladeRenderer.h>

/**
 * Should be called only from a rendering thread
 */
void Context::processRequests(void)
{
  if (stopRequested) {
    log("Context stop requested");
    stopRequested = false;
    clearNowFully();

    if (currentScene.get() != nullptr) {
      currentScene->shutdown(*this);
      currentScene.reset();
    }

    gladen::system::exit();

    return;
  }

  if (requestedScene.get() != nullptr) {
    log("Scene switch requested");
    switchScene();

    return;
  }

  if (clearRequested) {
    log("Clear requested");
    clearNowFully();
  }

  bool gladeObjectsListsChanged = false;

  if (!objectsToRemove.empty()) {
    log("Context: some objects will be deleted");
    gladeObjectsListsChanged = true;
    
    while (!objectsToRemove.empty()) {
      removeNow(objectsToRemove.front());
      objectsToRemove.pop();
    }
  }
  
  if (!objectsToAdd.empty()) {
    log("Context: new objects will be loaded");
    
    while (!objectsToAdd.empty()) {
      addNow(objectsToAdd.front());
      objectsToAdd.pop();
    }
    
    gladeObjectsListsChanged = true;
  }

  if (!widgetsToAdd.empty()) {
    log("Context: new widgets will be loaded");
    gladeObjectsListsChanged = true;
    
    while (!widgetsToAdd.empty()) {
      addNow(widgetsToAdd.front());
      widgetsToAdd.pop();
    }
  }

  if (gladeObjectsListsChanged) {
    log("Sorting drawables");
    renderer->sortDrawables();
  }
}

/**
 * Should not be called when containers are iterating
 */
void Context::removeNow(GladeObject* object)
{
  renderer->remove(object);
  simulator.remove(object);
  collisionDetector.remove(object);
  //aiContainer->remove(object);
  //soundPlayer->remove(object.getSounds());
}

/**
 * Should not be called when containers are iterating
 */
void Context::addNow(GladeObject* object)
{
  renderer->add(object);
  simulator.add(object);
  collisionDetector.add(object);
  //aiContainer->add(object);
  //soundPlayer->hold(object.getSounds());
}

/**
 * Should be called only from rendering thread
 * and not when containers are iterating
 */
void Context::addNow(Widget* root)
{
  // Consider this a root widget
  log("Root widget '%s' will be added", root->getName()->c_str());
  Transform rootWidgetTransform = renderer->getTransformForRootWidget();
  root->getTransform()->set(rootWidgetTransform);

  class CalculateWidgetTransforms : public Widget::WalkFunctor
  {
    public:
      virtual void operator()(Widget &widget)
      {
        assert(widget.getLayout() != nullptr);
        widget.getLayout()->get()->calculateTransformsForDirectChildrenOf(&widget);
      }
  } calculateWidgetTransforms;

  class AddWidgetsRecursive : public Widget::WalkFunctor
  {
    private:
      Context &context;
    public:
      AddWidgetsRecursive(Context &context): context(context) {}

      virtual void operator()(Widget &widget) {
        log("Adding widget %s", widget.getName()->c_str());
        context.renderer->add(&widget);
      }
  } addWidgetsRecursive(*this);

  Widget::walkDepthFirstPrefix(*root, calculateWidgetTransforms);
  Widget::walkDepthFirstPostfix(*root, addWidgetsRecursive);
}

/**
 * Should be called only from rendering thread. Unconditionally force clear all resources
 */
void Context::clearNowFully(void)
{
  log("Clearing fully");

  renderer->clear();
  //soundPlayer->clear(true);
  simulator.clear();
  collisionDetector.clear();
  aiContainer.clear();

  clearRequested = false;
}

/**
 * Should be called only from rendering thread. Invoke before state init to clear some resources
 */
void Context::clearBeforeSceneInit(void)
{
  log("Clearing before state init");

  renderer->clear();
  simulator.clear();
  collisionDetector.clear();
  aiContainer.clear();

  //soundPlayer->unholdAll();
}
