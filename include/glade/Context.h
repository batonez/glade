#pragma once

#include <queue>
#include <assert.h>

#include "system.h"
#include "render/GladeRenderer.h"
#include "State.h"
#include "ui/layout/Layout.h"
#include "ui/Widget.h"
#include "controls/VirtualController.h"
#include "physics/Simulator.h"
#include "physics/CollisionDetector.h"
#include "ai/AiContainer.h"
#include "Timer.h"
#include "debug/log.h"
//#include "#audio/SoundPlayer.h"

class Context {
public:
  Timer timer;
  GladeRenderer* renderer;
  //SoundPlayer* soundPlayer;

  // these are instantiated here because there's only one implementation now
  Simulator simulator;
  CollisionDetector collisionDetector;
  AiContainer aiContainer;

  bool enableSimulator, enableCollisionDetector, enableAiContainer, enableSoundPlayer;

private:
  std::unique_ptr<State> currentState, requestedState;
  bool stopRequested, clearRequested;
  VirtualController* controller;
  std::queue<GladeObject*> objectsToAdd;
  std::queue<Widget*> widgetsToAdd;
  std::queue<GladeObject*> objectsToRemove;
  std::queue<Widget*> widgetsToRemove;

public:
  Context(GladeRenderer* renderer/*, SoundPlayer* soundPlayer*/):
    renderer(renderer),
    //soundPlayer(soundPlayer),
    enableSimulator(true),
    enableCollisionDetector(true),
    enableAiContainer(true),
    enableSoundPlayer(true),
    stopRequested(false),
    clearRequested(false)
  {
  }

  void requestStateChange(std::unique_ptr<State> &state) {
    requestedState = std::move(state);
  }

  void requestStop(void) {
    stopRequested = true;
  }

  void add(GladeObject* object) {
    objectsToAdd.push(object);
  }

  void add(Widget* widget) {
    log("Setting UI");
    widgetsToAdd.push(widget);
  }

  void remove(GladeObject* object) {
    objectsToRemove.push(object);
  }

  void remove(Widget* widget) {
    widgetsToRemove.push(widget);
  }

  State* getCurrentState(void) {
    return currentState.get();
  }

  Simulator* getSimulator(void) {
    return &simulator;
  }

  CollisionDetector* getCollisionDetector(void) {
    return &collisionDetector;
  }

  AiContainer* getAiContainer(void) {
    return &aiContainer;
  }

  /**
   * Should be called only from a rendering thread
   */
  void processRequests(void) {
    if (stopRequested) {
      log("Context stop requested");
      stopRequested = false;
      clearNowFully();

      if (currentState.get() != nullptr) {
        currentState->shutdown(*this);
        currentState.reset();
      }

      gladen::system::exit();

      return;
    }

    if (requestedState.get() != nullptr) {
      log("State switch requested");
      switchState();

      return;
    }

    if (clearRequested) {
      log("Clear requested");
      clearNowFully();
    }

    bool gladeObjectsListsChanged = false;

    if (!objectsToAdd.empty()) {
      log("Context: new objects will be loaded");
      gladeObjectsListsChanged = true;
    }

    while (!objectsToAdd.empty()) {
      addNow(objectsToAdd.front());
      objectsToAdd.pop();
    }

    if (!widgetsToAdd.empty()) {
      log("Context: new widgets will be loaded");
      gladeObjectsListsChanged = true;
    }

    while (!widgetsToAdd.empty()) {
      addNow(widgetsToAdd.front());
      widgetsToAdd.pop();
    }

    if (gladeObjectsListsChanged) {
      log("Sorting drawables");
      renderer->sortDrawables();
    }
  }

  GladeRenderer* getRenderer(void) {
    return renderer;
  }

/*  SoundPlayer* getSoundPlayer(void) {
    return soundPlayer;
  }*/

  void clear(void) {
    clearRequested = true;
  }

  void setController(VirtualController &controller) {
    controller.init();
    this->controller = &controller;
  }

  VirtualController* getController(void) {
    return controller;
  }

private:
  /**
   * Should be called only from rendering thread
   */
  void switchState(void) {
    if (currentState.get() != nullptr) {
      clearBeforeStateInit();
      log("Shutting down current state");
      currentState->shutdown(*this);
      log("Current state was shut down");

      if (requestedState.get() != nullptr) {
        log("Initializing requested state");
        currentState = std::move(requestedState);
        currentState->init(*this);
      }

      clearAfterStateInit();
    } else {
      clearNowFully();

      if (requestedState.get() != nullptr) {
        currentState = std::move(requestedState);
        currentState->init(*this);
      }
    }
  }

  /**
   * Should be called only from rendering thread
   */
  void addNow(GladeObject* object) {
    renderer->add(object);
    simulator.add(object);
    collisionDetector.add(object);
    //aiContainer->add(object);
    //soundPlayer->hold(object.getSounds());
  }

  /**
   * Should be called only from rendering thread
   */
  void addNow(Widget* root) {
    // Consider this a root widget
    log("Adding root widget '%s'", root->getName()->c_str());
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
  void clearNowFully(void) {
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
  void clearBeforeStateInit(void) {
    log("Clearing before state init");

    renderer->clear();
    simulator.clear();
    collisionDetector.clear();
    aiContainer.clear();

    //soundPlayer->unholdAll();
  }

  /**
   * Should be called only from rendering thread.
   * Invoke after State.init() to clear remaining resources that are were not requested to be held in State.init()
   */
  void clearAfterStateInit(void) {
    log("Clearing after state init");
    //soundPlayer->clear(false);
  }
};
