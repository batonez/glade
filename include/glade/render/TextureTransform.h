#pragma once

#include <vector>

#include <glade/Timer.h>
#include <glade/Callable.h>
#include <glade/render/Texture.h>

class TextureTransform {
  public:
    // FIXME do we really need this decorator architecture?
    class AnimationCallback: public Callable
    {
      public:
        // Negative value means it never triggers
        virtual int   getTriggeringFrame() { return -1;    }
        virtual float getTriggeringTime()  { return -1.0f; }
        
      protected:
        Callable *callable;
        
      public:
        AnimationCallback(Callable *callable_param = NULL):
          callable(callable_param)
        {}
        
        virtual ~AnimationCallback()
        {
          if (callable) {
            delete callable;
            callable = NULL;
          }
        }
        
        virtual void call()
        {
          if (callable) {
            callable->call();
          }
        }
        
        virtual void call(TextureTransform &textureTransform) { call(); }
    };
  
    float animationTime;
    Timer timer;
    bool animating;
    float textureScaleX, textureScaleY;
  
  protected:
    typedef std::vector<AnimationCallback*> Callbacks;
  
    unsigned int currentFrameWidthPixels, currentFrameHeightPixels;
    unsigned char currentAnimationNumber;
    unsigned char currentFrameNumber;
    bool repeat;
    Callbacks callbacks;

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
    {
      // Executing frame and time hooked callbacks 
      Callbacks::iterator i = callbacks.begin();
      
      while (i != callbacks.end()) {
        if ((*i)->getTriggeringFrame() == currentFrameNumber) {
          (*i)->call(*this);
          delete *i;
          callbacks.erase(i);
        } else if (timer.getDeltaTime() == (*i)->getTriggeringTime()) {
          (*i)->call(*this);
          delete *i;
          callbacks.erase(i);
        } else {
          ++i;
        }
      }
    }
  
    void animate(int numberOfAnimation, float animationTime, bool repeat, bool forceStartFromZeroFrame = false)
    {
      if (currentAnimationNumber != numberOfAnimation || forceStartFromZeroFrame) {
        setCurrentAnimationNumber(numberOfAnimation);
        setCurrentFrameNumber(0);
      }
      
      this->animationTime = animationTime;
      this->repeat = repeat;
      animating = true;
      timer.reset();
      timer.unpause();
    }
    
    void freezeAnimation(bool freeze)
    {
      if (freeze) {
        timer.pause();
      } else {
        timer.reset();
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
    
    void flipTextureHorizontally(bool flip_param) {
       if (isFlippedHorizontally() != flip_param) {
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
  
    void addAnimationCallback(AnimationCallback *callback) {
      callbacks.push_back(callback);
    }
  
    //void removeAnimationCallback(AnimationCallback *callback) {
    //  callbacks.erase(callback);
    //}
    
    void removeAnimationCallbacks(void) {
      callbacks.clear();
    }
    
protected:
  void changeTextureFrames(Texture &texture) {
      if (timer.getDeltaTime() > animationTime / (float) texture.numberOfFrames) {
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
