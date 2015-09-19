#pragma once

#include "../Timer.h"

class TextureTransform {
public:
    float animationTime;
    Timer timer;
    bool animating;
    float textureScaleX, textureScaleY;
	
protected:
    unsigned int currentFrameWidthPixels, currentFrameHeightPixels;
    unsigned char currentAnimationNumber;
    unsigned char currentFrameNumber;
    bool repeat;
//  List<TextureAnimationCallback> callbacks = new ArrayList<TextureAnimationCallback>();

public:
	TextureTransform(void):
		animationTime(600.0f),
		animating(false),
		textureScaleX(1),
		textureScaleY(1),
		currentFrameWidthPixels(0),
		currentFrameHeightPixels(0),
		currentAnimationNumber(0),
		currentFrameNumber(0),
		repeat(true)
	{}
	
    void executeCallbacks(void) {
    	// Executing frame and time hooked callbacks
    	/*
		Iterator<TextureAnimationCallback> i = callbacks.iterator();
		TextureAnimationCallback callback;
		
		while (i.hasNext()) {
			callback = i.next();
			
			if (callback != null && callback.getTriggeringFrame() != null && callback.getTriggeringFrame() == currentFrameNumber) {
	    		i.remove();
				callback.call(this);
	    	}
			
			if (callback != null && callback.getTriggeringTime() != null && timer.getDeltaTime() > callback.getTriggeringTime()) {
				i.remove();
	    		callback.call(this);
	    	}
		} */
    }
	
    void animate(int numberOfAnimation, float animationTime, bool repeat, bool forceZeroFrame = false) {
    	if (currentAnimationNumber != numberOfAnimation || forceZeroFrame) {
    		setCurrentAnimationNumber(numberOfAnimation);
    		setCurrentFrameNumber(0);
    	}
    	
    	this->animationTime = animationTime;
    	this->repeat = repeat;
    	animating = true;
    	timer.unpause();
    }
    
    void freezeAnimation(bool freeze) {
    	if (freeze) {
    		timer.pause();
    	} else {
    		timer.unpause();
    	}
    }
    
    void setCurrentAnimationNumber(int numberOfAnimation) {
   		currentAnimationNumber = numberOfAnimation;
    }
    
    void setCurrentFrameNumber(int numberOfFrame) {
   		currentFrameNumber = numberOfFrame;
    }
    
    int getCurrentFrameNumber(Texture &texture) {
    	if (animating) {
    		changeTextureFrames(texture);
    	}
    	
    	return textureScaleX > 0 ? currentFrameNumber : texture.numberOfFrames - currentFrameNumber - 1;
    }
    
    int getCurrentAnimationNumber(Texture &texture) {
    	return currentAnimationNumber;
    }
    
    void flipTextureHorizontally(void) {
   		if (!isFlippedHorizontally()) {
   			textureScaleX = -textureScaleX;
   		}
    }
    
    void unflipTextureHorizontally(void) {
    	if (isFlippedHorizontally()) {
    		textureScaleX = -textureScaleX;
    	}
    }
    
    bool isFlippedHorizontally(void) {
    	return textureScaleX < 0;
    }
    
    void setCurrentFrameWidth(int frameWidthPixels) {
    	this->currentFrameWidthPixels = frameWidthPixels;
    }
    
    void setCurrentFrameHeight(int frameHeightPixels) {
    	this->currentFrameHeightPixels = frameHeightPixels;
    }
    
    float getTextureScaleXModifierForFrame(Texture &texture) {
    	return currentFrameWidthPixels != 0 && currentFrameWidthPixels != 0 ?
    		currentFrameWidthPixels / texture.frameWidth :
    		1;
    }
    
    float getTextureScaleYModifierForFrame(Texture &texture) {
    	return currentFrameHeightPixels != 0 && currentFrameHeightPixels != 0 ?
    		currentFrameHeightPixels / texture.frameHeight :
    		1;
    }
    
	/*
    void addAnimationCallback(TextureAnimationCallback callback) {
    	callbacks.add(callback);
    }
	
    void removeAnimationCallback(TextureAnimationCallback callback) {
    	callbacks.remove(callback);
    }*/
    
    void removeAnimationCallbacks(void) {
    //	callbacks.clear();
    }
	
protected:
	void changeTextureFrames(Texture &texture) {
    	if (timer.getDeltaTime() > animationTime / texture.numberOfFrames) {
			if (currentFrameNumber >= texture.numberOfFrames - 1) {
	    		if (repeat) {
	    			currentFrameNumber = 0;
	    		} else {
	    			animating = false;
	    		}
	    	} else {
	    		currentFrameNumber++;
	    	}
			
	    	timer.reset();
    	}
    }
};
