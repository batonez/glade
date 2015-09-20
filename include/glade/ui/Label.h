#pragma once

#include "Widget.h"
#include "font/BitmapFont.h"
#include "layout/CenterLayout.h"

class Label : public Widget
{
  private:
    std::string text;
    
  public:
    Label(const std::string &string, std::shared_ptr<BitmapFont> &font, float r = 0.0f, float g = 0.0f, float b = 0.0f): Widget()
    {
      Layout::Unique layout(new CenterLayout());
      setLayout(layout);
      
      setFocusable(false);
      addDrawables(*font->createDrawablesForString(string));
      getTransform()->getScale()->set(font->getStringWidth(string), font->getFontSize(), 1);
      setTextColor(r, g, b);

      text = string;
    }
    
    virtual void addDrawable(Drawable* view)
    {
      Widget::addDrawable(view);
      view->preserveScale = true;
    }
    
    void setTextColor(float r, float g, float b)
    {
      for (DrawablesI di = drawables.begin(); di != drawables.end(); ++di) {
        (*di)->setUniform("uColor", Vector4f(r, g, b, 1.0f));
      }
    }
    
    std::string getText()
    {
      return text;
    }
};
