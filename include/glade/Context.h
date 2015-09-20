#pragma once

#include <queue>

#include "system.h"
#include "render/GladeRenderer.h"
#include "State.h"
#include "ui/layout/Layout.h"
#include "controls/VirtualController.h"
//#include "../physics/Simulator.h"
//#include "../physics/CollisionDetector.h"
//#include "../ai/AiContainer.h"
//#include "#audio/SoundPlayer.h"

class Context {
public:
  Timer timer;
	GladeRenderer* renderer;
	//SoundPlayer* soundPlayer;
  
  // these are instantiated here becouse there's only one implementation now
	//Simulator simulator;
	//CollisionDetector collisionDetector;
  //AiContainer aiContainer;
	
  bool enableSimulator, enableCollisionDetector, enableAiContainer, enableSoundPlayer;

private:
	State::Unique currentState, requestedState;
	bool stopRequested, clearRequested;
	VirtualController* controller;
	queue<GladeObject*> objectsToAdd;
	queue<Widget*> widgetsToAdd;
	queue<GladeObject*> objectsToRemove;
	queue<Widget*> widgetsToRemove;
	
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
		
	void requestStateChange(State::Unique &state) {
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
	/*
	Simulator* getSimulator(void) {
		return &simulator;
	}
	
	CollisionDetector* getCollisionDetector(void) {
		return &collisionDetector;
	}*/
	
  /*
	AiContainer* getAiContainer(void) {
		return &aiContainer;
	}*/
	
	/**
	 * Should be called only from a rendering thread
	 */
	void processRequests(void) {
		if (stopRequested) {
      log("Context stop requested");
			stopRequested = false;
			clearNowFully();
      
      if (currentState.get() != nullptr) {
        currentState->shutdown();
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
		
    log("Sorting drawables");
		if (gladeObjectsListsChanged) {
			renderer->sortDrawables();
		}
	}
	
	GladeRenderer* getRenderer(void) {
		return renderer;
	}
	
/*	SoundPlayer* getSoundPlayer(void) {
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
      currentState->shutdown();
      log("Current state was shut down");
      
      if (requestedState.get() != nullptr) {
        log("Initializing requested state");
        currentState = std::move(requestedState);
        currentState->init();
      }
			
      clearAfterStateInit();
		} else {
			clearNowFully();
      
      if (requestedState.get() != nullptr) {
        currentState = std::move(requestedState);
        currentState->init();
      }
		}
	}
	
	/**
	 * Should be called only from rendering thread
	 */
	void addNow(GladeObject* object) {
    renderer->add(object);
    //simulator->add(object);
    //collisionDetector->add(object);
    //aiContainer->add(object);

    //soundPlayer->hold(object.getSounds());
	}
	
	/**
	 * Should be called only from rendering thread
	 */
	void addNow(Widget* root) {
		// Consider this a root widget
    log("Adding root widget");
		root->getTransform()->set(renderer->getTransformForRootWidget());
		root->getLayout()->calculateTransformsForChildrenOf(root); // TODO this should not be a recursive call. Instead pack this into the walker
    
    class AddWidgetsRecursive : public Widget::WalkFunctor {
      private:
        Context &context;
      public:
        AddWidgetsRecursive(Context &context): context(context) {}
        
        virtual void operator()(Widget &widget) {
          context.renderer->add(&widget);
        }
    } addWidgetsRecursive(*this);
    
    Widget::walkDepthFirstPostfix(*root, addWidgetsRecursive);
	}
	
	/**
	 * Should be called only from rendering thread. Unconditionally force clear all resources
	 */
	void clearNowFully(void) {
		log("Clearing fully");
		
		renderer->clear();
		//soundPlayer->clear(true);
		//simulator.clear();
		//collisionDetector.clear();
		//aiContainer.clear();
		
		clearRequested = false;
	}
	
	/**
	 * Should be called only from rendering thread. Invoke before state init to force clear some resources
	 */
	void clearBeforeStateInit(void) {
		log("Clearing before state init");
		
		renderer->clear();
		//simulator.clear();
		//collisionDetector.clear();
		//aiContainer.clear();
		
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