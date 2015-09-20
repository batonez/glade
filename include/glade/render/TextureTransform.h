#pragma once

#include "../Timer.h"
#include "../Callable.h"

class TextureTransform {
  public:/*
    class AnimationCallback: public Callable
    {
      public:
        // Negative value means it never triggers
        virtual int   getTriggeringFrame();
        virtual float getTriggeringTime();
        
      protected:
        std::unique_ptr<Callable> callable;
        
      public:
        AnimationCallback() {}
        
        AnimationCallback(std::unique_ptr<Callable> &callable_param)
        {
          //callable = std::move(callable_param);
        }
        
        virtual void call()
        {
          if (callable != nullptr) {
            callable->call();
          }
        }
        
        virtual void call(TextureTransform &textureTransform) { call(); }
    };*/
  
    float animationTime;
    Timer timer;
    bool animating;
    float textureScaleX, textureScaleY;
  
  protected:
    //typedef std::vector<std::unique_ptr<AnimationCallback> > Callbacks;
  
    unsigned int currentFrameWidthPixels, currentFrameHeightPixels;
    unsigned char currentAnimationNumber;
    unsigned char currentFrameNumber;
    bool repeat;
    //Callbacks callbacks;

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
  
    void executeCallbacks(void)
    {/*
      // Executing frame and time hooked callbacks 
      for (Callbacks::iterator i = callbacks.begin(); i != callbacks.end(); ++i) {
        if ((*i)->getTriggeringFrame() == currentFrameNumber) {
          (*i)->call(*this);
          callbacks.erase(i);
        }
        
        if (timer.getDeltaTime() == (*i)->getTriggeringTime()) {
          (*i)->call(*this);
          callbacks.erase(i);
        }
      }*/
    }
  
    void animate(int numberOfAnimation, float animationTime, bool repeat, bool forceZeroFrame = false)
    {
      if (currentAnimationNumber != numberOfAnimation || forceZeroFrame) {
        setCurrentAnimationNumber(numberOfAnimation);
        setCurrentFrameNumber(0);
      }
      
      this->animationTime = animationTime;
      this->repeat = repeat;
      animating = true;
      timer.unpause();
    }
    
    void freezeAnimation(bool freeze)
    {
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
  
    //void addAnimationCallback(std::unique_ptr<AnimationCallback> &callback) {
      //callbacks.push_back(std::move(callback));
    //}
  
    //void removeAnimationCallback(AnimationCallback callback) {
     // callbacks.erase(callback);
    //}
    
    void removeAnimationCallbacks(void) {
     // callbacks.clear();
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
