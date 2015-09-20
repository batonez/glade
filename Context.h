#pragma once

#include <queue>

#include "system.h"

class Context {
public:
  Timer timer;
	GladeRenderer* renderer;
	SoundPlayer* soundPlayer;
  
  // these are instantiated here becouse there's only one implementation now
	Simulator simulator;
	CollisionDetector collisionDetector;
	AiContainer aiContainer;
	
  bool enableSimulator, enableCollisionDetector, enableAiContainer, enableSoundPlayer;

private:
	State* currentState, requestedState;
	bool stopRequested, clearRequested;
	VirtualController* controller;
	queue<GladeObject*> objectsToAdd;
	queue<Widget*> widgetsToAdd;
	queue<GladeObject*> objectsToRemove;
	queue<Widget*> widgetsToRemove;
	
public:
	Context(GladeRenderer* renderer, SoundPlayer* soundPlayer):
    renderer(renderer),
    soundPlayer(soundPlayer),
		enableSimulator(true),
		enableCollisionDetector(true),
		enableAiContainer(true),
		enableSoundPlayer(true),
    stopRequested(false),
    clearRequested(false)
	{
	}
		
	void requestStateChange(State* state) {
		requestedState = state;
	}

	void requestStop(void) {
		stopRequested = true;
	}
	
	void add(GladeObject* object) {
		objectsToAdd.addLast(object);
	}
	
	void add(Widget* widget) {
		widgetsToAdd.addLast(widget);
	}
	
	void remove(GladeObject* object) 
		objectsToRemove.add(object);
	}
	
	void remove(Widget* widget) {
		widgetsToRemove.add(widget);
	}
	
	State* getCurrentState(void) {
		return currentState;
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
			stopRequested = false;
			clearNowFully();
			gladen::system::exit();
			
			return;
		}
		
		if (requestedState != NULL) {
			switchState();
			
			return;
		}
		
		if (clearRequested) {
			clearNowFully();
		}
				
		bool gladeObjectsListsChanged = false;
		
		while (!objectsToAdd.empty()) {
			addNow(objectsToAdd.front());
			objectsToAdd.pop();
			gladeObjectsListsChanged = true;
		}
		
		while (!widgetsToAdd.empty()) {
			addNow(widgetsToAdd.front());
			widgetsToAdd.pop();
			gladeObjectsListsChanged = true;
		}
		
		if (gladeObjectsListsChanged) {
			renderer.sortDrawables();
		}
	}
	
	GladeRenderer* getRenderer(void) {
		return renderer;
	}
	
	SoundPlayer* getSoundPlayer(void) {
		return soundPlayer;
	}
	
	void clear(void) {
		clearRequested = true;
	}
	
	void setController(VirtualController* controller) {
		controller.init();
		this->controller = controller;
	}
	
	VirtualController* getController(void) {
		return controller;
	}
	
	void pollInput(void) {
		gladen::system::pollInput();
	}

private:
	/**
	 * Should be called only from rendering thread
	 */
	void switchState(void) {
		if (requestedState == NULL) {
			log("You should request a state before activating it");
      return;
		}
		
		currentState = requestedState;
		requestedState = NULL;
		
		if (currentState != NULL) {
			clearBeforeStateInit();
			currentState->init();
			clearAfterStateInit();
		} else {
			clearNowFully();
		}
	}
	
	/**
	 * Should be called only from rendering thread
	 */
	void addWidgetsRecursive(Widget &widget) {
		renderer->add(widget);
		Iterator<Widget> children = widget.getChildrenIterator(); // ? TODO (Implement ui first)
		
		while (children.hasNext()) {
			addWidgetsRecursive(children.next());
		}
	}
	
	/**
	 * Should be called only from rendering thread
	 */
	void addNow(GladeObject &object) {
		renderer->add(object);
		simulator->add(object);
		collisionDetector->add(object);
		aiContainer->add(object);
		
		soundPlayer->hold(object.getSounds());
	}
	
	/**
	 * Should be called only from rendering thread
	 */
	void addNow(Widget &widget) {
		if (widget.getParent() == NULL) {
			widget.getTransform().setComponentsTo(renderer.getTransformForRootWidget());
		}
		
		widget.getLayout()->calculateTransformsForChildrenOf(widget);
		addWidgetsRecursive(widget);
	}
	
	/**
	 * Should be called only from rendering thread. Unconditionally force clear all resources
	 */
	void clearNowFully(void) {
		log("Clearing fully");
		
		renderer->clear();
		soundPlayer->clear(true);
		simulator.clear();
		collisionDetector.clear();
		aiContainer.clear();
		
		clearRequested = false;
	}
	
	/**
	 * Should be called only from rendering thread. Invoke before state init to force clear some resources
	 */
	void clearBeforeStateInit(void) {
		log("Clearing before state init");
		
		renderer->clear();
		simulator.clear();
		collisionDetector.clear();
		aiContainer.clear();
		
		soundPlayer->unholdAll();
	}
	
	/**
	 * Should be called only from rendering thread.
	 * Invoke after State.init() to clear remaining resources that are were not requested to be held in State.init()
	 */
	void clearAfterStateInit(void) {
		log("Clearing after state init");
		soundPlayer->clear(false);
	}
};