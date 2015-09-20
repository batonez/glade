#pragma once

#include "Widget.h"
#include "font/BitmapFont.h"
#include "layout/CenterLayout.h"

class Label : public Widget
{
	public:
    Label(std::string &string, BitmapFont &font) : Label(string, font, 0.0f, 0.0f, 0.0f)
    {
    }
    
    Label(std::string &string, BitmapFont &font, float r, float g, float b) : Widget(CenterLayout::INSTANCE)
    {
      setFocusable(false);

      addDrawables(*font.createDrawablesForString(string));
      getTransform()->getScaleP()->set(font.getStringWidth(string), font.getFontSize(), 1);
//    setTextColor(r, g, b); // MAKE SHADERS FOR TEXT AND DEBUG THEM
    }
    
    Label() : Widget(CenterLayout::INSTANCE)
    {
      setFocusable(false);
    }
    
    void addDrawable(Drawable* view)
    {
      Widget::addDrawable(view);
      view->preserveScale = true;
    }
    
    void setTextColor(float r, float g, float b)
    {
      for (DrawablesI di = drawables.begin(); di != drawables.end(); ++di) {
        BasicTextShaderProgram *program = dynamic_cast<BasicTextShaderProgram*>((*di)->getShaderProgram());
        program->replaceOriginalColor = true;
        program->constantColor.set(r, g, b, 1.0f);
      }
    }
};
