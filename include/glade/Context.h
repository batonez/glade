#pragma once

#include <queue>
#include <assert.h>

#include "system.h"
#include "Scene.h"
#include "ui/layout/Layout.h"
#include "ui/Widget.h"
#include "controls/VirtualController.h"
#include "physics/Simulator.h"
#include "physics/CollisionDetector.h"
#include "ai/AiContainer.h"
#include "Timer.h"
#include "debug/log.h"
//#include "#audio/SoundPlayer.h"

namespace Glade {
  class Renderer;
}

class Context {
public:
  Timer timer;
  Glade::Renderer* renderer;
  //SoundPlayer* soundPlayer;

  // these are instantiated here because there's only one implementation now
  Simulator simulator;
  CollisionDetector collisionDetector;
  AiContainer aiContainer;

  bool enableSimulator, enableCollisionDetector, enableAiContainer, enableSoundPlayer;

private:
  std::unique_ptr<Scene> currentScene, requestedScene;
  bool stopRequested, clearRequested;
  VirtualController* controller;
  std::queue<GladeObject*> objectsToAdd;
  std::queue<Widget*> widgetsToAdd;
  std::queue<GladeObject*> objectsToRemove;
  std::queue<Widget*> widgetsToRemove;

public:
  Context(Glade::Renderer* renderer/*, SoundPlayer* soundPlayer*/):
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

  void requestSceneChange(std::unique_ptr<Scene> &state) {
    requestedScene = std::move(state);
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

  Scene* getCurrentScene(void) {
    return currentScene.get();
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
  void processRequests(void);

  Glade::Renderer* getRenderer(void) {
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
  void switchScene(void) {
    if (currentScene.get() != nullptr) {
      clearBeforeSceneInit();
      log("Shutting down current state");
      currentScene->shutdown(*this);
      log("Current state was shut down");

      if (requestedScene.get() != nullptr) {
        log("Initializing requested state");
        currentScene = std::move(requestedScene);
        currentScene->init(*this);
      }

      clearAfterSceneInit();
    } else {
      clearNowFully();

      if (requestedScene.get() != nullptr) {
        currentScene = std::move(requestedScene);
        currentScene->init(*this);
      }
    }
  }

  /**
   * Should not be called when containers are iterating
   */
  void removeNow(GladeObject* object);
  
  /**
   * Should not be called when containers are iterating
   */
  void addNow(GladeObject* object);

  /**
   * Should be called only from rendering thread
   * and not when containers are iterating
   */
  void addNow(Widget* root);

  /**
   * Should be called only from rendering thread. Unconditionally force clear all resources
   */
  void clearNowFully(void);

  /**
   * Should be called only from rendering thread. Invoke before state init to clear some resources
   */
  void clearBeforeSceneInit(void);

  /**
   * Should be called only from rendering thread.
   * Invoke after Scene.init() to clear remaining resources that are were not requested to be held in Scene.init()
   */
  void clearAfterSceneInit(void) {
    log("Clearing after state init");
    //soundPlayer->clear(false);
  }
};
