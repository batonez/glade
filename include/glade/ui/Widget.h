#pragma once

#include <set>
#include <list>

#include "../GladeObject.h"
#include "../Callable.h"
#include "../math/Transform.h"

class Layout;

// TODO Бесконечная рекурсия при попытке смены фокуса, когда focusable элементов вообще нет
class Widget : public GladeObject
{
// Tree functions
  public:
    typedef std::list<Widget*> Children;
    typedef Children::iterator ChildrenI;

    class WalkFunctor
    {
      public:
        virtual void operator()(Widget &widget) = 0;
    };
    
    static void walkDepthFirstPostfix(Widget &widget, WalkFunctor &func)
    {
      ChildrenI child = widget.getChildren()->begin();
          
      while (child != widget.getChildren()->end()) {
        walkDepthFirstPostfix(**child, func);
        ++child;
      }
      
      func(widget);
    }
    
  private:
    Children  children;
 
  public:
    void addChild(Widget *child) { children.push_back(child); }
    Children* getChildren() { return &children; }
// End of tree functions


  protected:
    bool focusable;
    
  private:
    typedef GladeObject super;
    
	  Layout &layout;
	  Drawables unfocusedViews;
	  Drawables focusedViews;
	  bool focused;
	  Callable::Callables onFocusListeners;
	  Callable::Callables onBlurListeners;
	  Callable::AutoDispose action;
  
  public:
    Widget(Layout &layout):
      focused(false),
      focusable(true),
      layout(layout)
    {
    }
    
    void addFocusedView(Drawable* view)
    {
      addDrawable(view);
      toggleView(*view, focused);
      focusedViews.insert(view);
    }
    
    void addUnfocusedView(Drawable* view)
    {
      addDrawable(view);
      toggleView(*view, !focused);
      unfocusedViews.insert(view);
    }
    
    bool isPointInside(float x, float y)
    {
      Transform::SharedVector position = getTransform()->getPosition();
      Transform::SharedVector scale = getTransform()->getScale();
      
      if (x > position->x - scale->x && x < position->x + scale->x &&
        y > position->y - scale->y && y < position->y + scale->y
      ) {
        return true;
      }
      
      return false;
    }
    
    Widget* getWidgetAtPoint(float x, float y)
    {
      if (isPointInside(x, y)) {
        ChildrenI child = children.begin();
        Widget* result;
        
        while (child != children.end()) { // FIXME Если виджеты наслаиваются друг на друга, то порядок обхода может быть неверным
          result = (*child)->getWidgetAtPoint(x, y);
          
          if (result != NULL && result->isFocusable()) {
            return result;
          }

          ++child;
        }
        
        if (this->isFocusable()) {
          return this;
        }
      }
      
      return NULL;
    }
    
    void setFocused(bool focused)
    {
      for (DrawablesI drawable = focusedViews.begin(); drawable != focusedViews.end(); ++drawable) {
        toggleView(**drawable, focused);
      }
      
      for (DrawablesI drawable = unfocusedViews.begin(); drawable != unfocusedViews.end(); ++drawable) {
        toggleView(**drawable, !focused);
      }

      if (focused && !this->focused) {
        for (Callable::CallablesI listener = onFocusListeners.begin(); listener != onFocusListeners.end(); ++listener) {
          (*listener)->call();
        }
      } else if (!focused && this->focused) {
        for (Callable::CallablesI listener = onBlurListeners.begin(); listener != onBlurListeners.end(); ++listener) {
          (*listener)->call();
        }
      }
      
      this->focused = focused;
    }
    
    Layout* getLayout()
    {
      return &layout;
    }
    
    bool isFocusable()
    {
      return focusable;
    }
    
    void setFocusable(bool focusable)
    {
      this->focusable = focusable;
    }
    
    void addOnFocusListener(Callable* listener)
    {
      onFocusListeners.insert(listener);
    }
    
    void addOnBlurListener(Callable* listener)
    {
      onBlurListeners.insert(listener);
    }
    
    void removeOnFocusListener(Callable* listener)
    {
      onFocusListeners.erase(listener);
    }
    
    void removeOnBlurListener(Callable* listener)
    {
      onBlurListeners.erase(listener);
    }
    
    void setAction(Callable::AutoDispose &action)
    {
      this->action = std::move(action);
    }
    
    void performAction()
    {
      if (action.get() != nullptr) {
        action->call();
      }
    }
};
