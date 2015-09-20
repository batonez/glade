#pragma once

#include "glade/math/Vector.h"
#include "glade/ui/Widget.h"

class Layout
{
  public:
    enum SizeType
    {
      MANUAL,
      PERCENT_OF_PARENT,
      PERCENT_OF_SELF_WIDTH,
      PERCENT_OF_SELF_HEIGHT,
      WRAP_CONTENT
    };
    
    enum HorizontalAlign
    {
      LEFT, RIGHT, CENTER
    };
    
    enum VerticalAlign
    {
      TOP, BOTTOM, MIDDLE
    };
    
	protected:
    Vector2f minContentPosition;
	  Vector2f maxContentPosition;
	  Vector4f padding;
	  Vector4f margin;
	  HorizontalAlign horizontalAlign;
	  VerticalAlign verticalAlign;
	  float horizontalSizePercent;
	  float verticalSizePercent;
	  SizeType horizontalSizeType;
	  SizeType verticalSizeType;
	
	public:
    Layout() : Layout(LEFT, TOP, MANUAL, MANUAL)
    {
    }
    
    Layout(HorizontalAlign hor, VerticalAlign vert, SizeType horSize, SizeType vertSize) :
      horizontalAlign(hor),
      verticalAlign(vert),
      horizontalSizeType(horSize),
      verticalSizeType(vertSize)
    {
    }
    
    virtual void calculateTransformsForChildrenOf(Widget* widget) = 0;
    
    void setVerticalAlign(VerticalAlign verticalAlign)
    {
      this->verticalAlign = verticalAlign;
    }
    
    void setHorizontalAlign(HorizontalAlign horizontalAlign)
    {
      this->horizontalAlign = horizontalAlign;
    }
    
    Vector4f* getPadding()
    {
      return &padding;
    }
    
    void setPadding(float top, float right, float bottom, float left)
    {
      padding.set(top, right, bottom, left);
    }
    
    Vector4f* getMargin()
    {
      return &margin;
    }
    
    void setMargin(float top, float right, float bottom, float left)
    {
      margin.set(top, right, bottom, left);
    }
    
    float getHorizontalSizePercent()
    {
      log("Inside getHorizontalSizePercent");
      return horizontalSizePercent;
    }
    
    void setHorizontalSizePercent(float horizontalSize)
    {
      this->horizontalSizePercent = horizontalSize;
    }
    
    float getVerticalSizePercent()
    {
      return verticalSizePercent;
    }
    
    void setVerticalSizePercent(float verticalSize)
    {
      this->verticalSizePercent = verticalSize;
    }
    
    void setHorizontalSizeType(SizeType sizeType)
    {
      if (sizeType == PERCENT_OF_SELF_WIDTH) {
        throw GladeException("Horizontal size cannot be specified as percents of itself.");
      }
      
      if (sizeType == PERCENT_OF_SELF_HEIGHT && verticalSizeType == PERCENT_OF_SELF_WIDTH) {
        throw GladeException("Cannot specify horizontal size as percents of self height,"
          " because height is already specified as percents of this horizontal size.");
      }
      
      this->horizontalSizeType = sizeType;
    }
    
    SizeType getHorizontalSizeType()
    {
      return horizontalSizeType;
    }
    
    void setVerticalSizeType(SizeType sizeType)
    {
      if (sizeType == PERCENT_OF_SELF_HEIGHT) {
        throw GladeException("Vertical size cannot be specified as percents of itself.");
      }
      
      if (sizeType == PERCENT_OF_SELF_WIDTH && verticalSizeType == PERCENT_OF_SELF_HEIGHT) {
        throw GladeException("Cannot specify vertical size as percents of self width,"
          " because width is already specified as percents of this vertical size.");
      }
      
      this->verticalSizeType = sizeType;
    }
    
    SizeType getVerticalSizeType()
    {
      return verticalSizeType;
    }
   
  protected:
    void rescaleChildrenOf(Widget* widget)
    {
      Widget::Widgets* children = widget->getChildren();
      
      for (Widget::WidgetsI child = children->begin(); child != children->end(); ++child) {
        if (!rescaleX(*child, widget)) {
          rescaleY(*child, widget);
        }
      }
    }
	
	  /** Возвращает true если в результате были расчитаны оба размера X и Y. Если был расчитан только X - возвращает false */
    bool rescaleX(Widget* widget, Widget* hisParent)
    {
      float horizontalSize = widget->getLayout()->getHorizontalSizePercent();
      bool result = false;
      
      switch (widget->getLayout()->getHorizontalSizeType()) {
        case PERCENT_OF_PARENT:
          widget->getTransform()->getScale()->x = horizontalSize * hisParent->getTransform()->getScale()->x - ((padding.y + padding.w) / 2);
          break;
        case PERCENT_OF_SELF_HEIGHT:
          rescaleY(widget, hisParent);
          widget->getTransform()->getScale()->x = horizontalSize * widget->getTransform()->getScale()->y;
          result = true;
          break;
        default:
          break;
      }
      
      return result;
    }
    
    /** Возвращает true если в результате были расчитаны оба размера X и Y. Если был расчитан только Y - возвращает false */
    bool rescaleY(Widget* widget, Widget* hisParent)
    {
      float verticalSize = widget->getLayout()->getVerticalSizePercent();
      bool result = false;
      
      switch (widget->getLayout()->verticalSizeType) {
        case PERCENT_OF_PARENT:
          widget->getTransform()->getScale()->y = verticalSize * hisParent->getTransform()->getScale()->y - ((padding.x + padding.z) / 2);
          break;
        case PERCENT_OF_SELF_WIDTH:
          rescaleX(widget, hisParent);
          widget->getTransform()->getScale()->y = verticalSize * widget->getTransform()->getScale()->x;
          result = true;
          break;
        default:
          break;
      }
      
      return result;
    }
};
