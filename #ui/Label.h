#pragma once

#include "Widget.h"
#include "font/BitmapFont.h"

class Label : public Widget
{
	public:
    // should be manually freed after use
    static Label* forString(std::string &string, BitmapFont &font, Layout* layout)
    {
      return forString(string, font, layout, 0.0f, 0.0f, 0.0f);
    }
    
    // should be manually freed after use
    static Label* forString(std::string &string, BitmapFont &font, Layout* layout, float r, float g, float b)
    {
      Label* label = new Label(layout);
      
      label->addDrawables(*font.createDrawablesForString(string));
      
      label->getTransform()->getScaleP()->set(font.getStringWidth(string), font.getFontSize(), 1);
//      label->setTextColor(r, g, b);
          
      return label;
    }
    
    // this layout pointer is not good here
    Label(Layout* layout) : Widget(layout)
    {
      focusable = false;
    }
    
    void addDrawable(Drawable* view)
    {
      Widget::addDrawable(view);
      view->preserveScale = true;
    }
    
    void setTextColor(float r, float g, float b)
    {
      for (DrawablesI di = drawables.begin(); di != drawables.end(); ++di) {
        (*di)->replaceOriginalColor = true;
        (*di)->setConstantColor(r, g, b);
      }
    }
};
