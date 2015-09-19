#pragma once

class Context {
public:
	GladeRenderer* renderer;
	SoundPlayer* soundPlayer;
	Simulator simulator;
	CollisionDetector collisionDetector;
	AiContainer aiContainer;
	bool enableSimulator, enableCollisionDetector, enableAiContainer, enableSoundPlayer;
	
protected:
	SystemInterface* system;
	
private:
	Timer timer;
	State* currentState, requestedState;
	bool stopRequested = false, clearRequested = false;
	VirtualController* controller;
	Deque<GladeObject> objectsToAdd = new ArrayDeque<GladeObject>();
	Deque<Widget> widgetsToAdd = new ArrayDeque<Widget>();
	Deque<GladeObject> objectsToRemove = new ArrayDeque<GladeObject>();
	Deque<Widget> widgetsToRemove = new ArrayDeque<Widget>();
	
public:
	Context(SystemInterface &system, GladeRenderer &renderer, SoundPlayer &soundPlayer):
		enableSimulator(true),
		enableCollisionDetector(true),
		enableAiContainer(true),
		enableSoundPlayer(true)
	{
		this->system = &system;
		this->renderer = &renderer;
		this->soundPlayer = &soundPlayer;

		controller = new VoidController();
	}
	
	~Context(void) {
		delete controller;
		delete requestedState;
		delete currentState;
		// clear queues
	}
	
	void requestStateChange(State &state) {
		requestedState = &state;
	}

	void requestStop(void) {
		stopRequested = true;
	}
	
	void add(GladeObject object) {
		objectsToAdd.addLast(object);
	}
	
	void add(Widget widget) {
		widgetsToAdd.addLast(widget);
	}
	
	void remove(GladeObject object) 
		objectsToRemove.add(object);
	}
	
	void remove(Widget widget) 
		widgetsToRemove.add(widget);
	}
	
	State* getCurrentState(void) {
		return currentState;
	}
	
	Simulator* getSimulator(void) {
		return simulator;
	}
	
	CollisionDetector* getCollisionDetector(void) {
		return collisionDetector;
	}
	
	AiContainer* getAiContainer(void) {
		return aiContainer;
	}
	
	/**
	 * Should be called only from a rendering thread
	 */
	void processRequests(void) {
		if (stopRequested) {
			stopRequested = false;
			clearNowFully();
			system.exit();
			
			return;
		}
		
		if (requestedState != NULL) {
			switchState();
			
			return;
		}
		
		if (clearRequested) {
			clearNowFully();
		}
		
		GladeObject object;
		Widget widget;
		
		bool gladeObjectsListsChanged = false;
		
		while ((object = objectsToAdd.peekFirst()) != NULL) {
			addNow(object);
			objectsToAdd.removeFirst();
			gladeObjectsListsChanged = true;
		}
		
		while ((widget = widgetsToAdd.peekFirst()) != NULL) {
			addNow(widget);
			widgetsToAdd.removeFirst();
			gladeObjectsListsChanged = true;
		}
		
		if (gladeObjectsListsChanged) {
			renderer.sortDrawables();
		}
	}
	
	double getDeltaTime(void) {
		return timer.getDeltaTime();
	}
	
	void resetTimer(void) {
		timer.reset();
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
	
	void setController(VirtualController &controller) {
		controller.init();
		this->controller = &controller;
	}
	
	VirtualController* getController(void) {
		return controller;
	}
	
	void pollInput(void) {
		system->pollInput();
	}

private:
	/**
	 * Should be called only from rendering thread
	 */
	void switchState(void) {
		if (requestedState == NULL) {
			//throw new RuntimeException("You should request a state before activating it");
		}
		
		delete currentState;
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
		renderer.add(widget);
		Iterator<Widget> children = widget.getChildrenIterator();
		
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
		
		widget.getLayout().calculateTransformsForChildrenOf(widget);
		addWidgetsRecursive(widget);
	}
	
	/**
	 * Should be called only from rendering thread. Unconditionally force clear all resources
	 */
	void clearNowFully(void) {
		System.out.println("Clearing fully");
		
		renderer.clear();
		soundPlayer.clear(true);
		simulator.clear();
		collisionDetector.clear();
		aiContainer.clear();
		
		clearRequested = false;
	}
	
	/**
	 * Should be called only from rendering thread. Invoke before state init to force clear some resources
	 */
	void clearBeforeStateInit(void) {
		System.out.println("Clearing before state init");
		
		renderer.clear();
		simulator.clear();
		collisionDetector.clear();
		aiContainer.clear();
		
		soundPlayer.unholdAll();
	}
	
	/**
	 * Should be called only from rendering thread.
	 * Invoke after State.init() to clear remaining resources that are were not requested to be held in State.init()
	 */
	void clearAfterStateInit(void) {
		System.out.println("Clearing after state init");
		soundPlayer.clear(false);
	}
}