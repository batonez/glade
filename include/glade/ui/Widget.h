#pragma once

#include <set>

#include "../GladeObject.h"
#include "../Callable.h"
#include "../math/Transform.h"

class Layout;

// TODO Бесконечная рекурсия при попытке смены фокуса, когда focusable элементов вообще нет
class Widget : public GladeObject
{
  public:
    typedef std::set<Widget*>  Widgets;
    typedef Widgets::iterator  WidgetsI;

  protected:
    bool focusable;
    
  private:
    typedef GladeObject super;
    
    Widgets children;
	  Widget* parent;
	  Layout &layout;
	  WidgetsI focusIterator;
	  Widget* breadCrumb;
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
      layout(layout),
      parent(NULL),
      breadCrumb(NULL)
    {
    }
    
    void setEnabled(bool enable)
    {
      super::setEnabled(enable);
      
      for (WidgetsI child = children.begin(); child != children.end(); ++child) {
        (*child)->setEnabled(enable);
      }
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
        WidgetsI child = children.begin();
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
    
    void setParent(Widget* widget)
    {
      if (parent != NULL) {
        parent->getChildren()->erase(this);
      }
      
      if (widget != NULL) {
        widget->getChildren()->insert(this);
      }
      
      parent = widget;
    }
    
    Widget* getParent()
    {
      return parent;
    }
    
    Widget* getFocusedElement()
    {
      if (breadCrumb == NULL) {
        return NULL;
      }
      
      if (breadCrumb == this) {
        return this;
      }
      
      return breadCrumb->getFocusedElement();
    }
    
    /**
     * Если пройдены все потомки - breadCrumb ставится в null, и возвращается false
     */
    bool focusNextDescendant()
    {
      // снимаем флаг фокуса с текущего сфокусированного элемента (ищем его от корня)
      if (getParent() == NULL) {
        Widget* focusedElement = getFocusedElement();
        
        if (focusedElement != NULL) {
          focusedElement->setFocused(false);
        }
      }
      
      bool result = true;
      
      // Если сфокусированный элемент - этот, то на этом шаге фокус просто убирается и родитель оповещается что в этой ветке элементы закончились
      if (breadCrumb == this) {
        breadCrumb = NULL;
        
        // если это корневой элемент - делаем еще одну итерацию (чтобы не случилось так что ни один элемент дерева не имеет фокус)
        if (getParent() == NULL) {
          focusNextDescendant();
        }
        
        result = false;
      } else {
        // Если фокус был не в нашей ветке, значит нужно создать новый итератор для обхода нашей ветки
        if (breadCrumb == NULL || focusIterator == children.end()) {
          focusIterator = children.begin();
        }
        
        // пробуем переключить фокус у первого прямого потомка. Если потомок возвращает false (т.е. у него закончились его потомки), то пробуем следующего
        while (breadCrumb == NULL || !breadCrumb->focusNextDescendant()) {
          Widget* newBreadCrumb = *focusIterator;
          
          if (breadCrumb != NULL && breadCrumb == newBreadCrumb) {
            ++focusIterator;
            
            if (focusIterator != children.end()) {
              newBreadCrumb = *focusIterator;
            }
          }
          
          breadCrumb = newBreadCrumb;
            
          if (focusIterator == children.end()) {
            if (!isFocusable()) { // если этот элемент не фокусируемый, то в этой ветке больше нечего фокусировать, оповещаем об этом вызвавшего родителя
              breadCrumb = NULL;
              
              if (getParent() == NULL) {
                focusNextDescendant();
              }
              
              result = false;
            } else { // если этот элемент фокусируемый - фокусируем его
              breadCrumb = this;
              result = true;
            }
            
            break;
          }
          
          ++focusIterator;
        }
      }
      
      // Устанавливаем флаг фокуса на новый найденный элемент
      if (getParent() == NULL) {
        Widget* focusedElement = getFocusedElement();
        
        if (focusedElement != NULL) {
          focusedElement->setFocused(true);
        }
      }
      
      return result;
    }
    
    /**
     * Если пройдены все потомки - breadCrumb ставится в null, и возвращается false
     */
    bool focusPreviousDescendant()
    {
      if (getParent() == NULL) {
        Widget* focusedElement = getFocusedElement();
        
        if (focusedElement != NULL) {
          focusedElement->setFocused(false);
        }
      }
      
      bool result = true;
      
      if (breadCrumb == this) {
        breadCrumb = NULL;
        
        if (getParent() == NULL) {
          focusPreviousDescendant();
        }
        
        result = false;
      } else {
        if (breadCrumb == NULL || focusIterator == children.end()) {
          focusIterator = --children.end();
        }
        
        while (breadCrumb == NULL || !breadCrumb->focusPreviousDescendant()) {
          Widget* newBreadCrumb = *focusIterator;
          
          if (breadCrumb != NULL && breadCrumb == newBreadCrumb) {
            --focusIterator;
            
            if (focusIterator != children.end()) {
              newBreadCrumb = *focusIterator;
            }
          }
          
          breadCrumb = newBreadCrumb;
          
          if (focusIterator == children.end()) {
            if (!isFocusable()) {
              breadCrumb = NULL;
              
              if (getParent() == NULL) {
                focusPreviousDescendant();
              }
              
              result = false;
            } else {
              breadCrumb = this;
              result = true;
            }
            
            break;
          }
          
          --focusIterator;
        }
      }
      
      if (getParent() == NULL) {
        Widget* focusedElement = getFocusedElement();
        
        if (focusedElement != NULL) {
          getFocusedElement()->setFocused(true);
        }
      }
      
      return result;
    }
    
    bool hasChildren()
    {
      return !getChildren()->empty();
    }
    
    bool isFocusable()
    {
      return focusable;
    }
    
    void setFocusable(bool focusable)
    {
      this->focusable = focusable;
    }
    
    Widgets* getChildren()
    {
      return &children;
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
