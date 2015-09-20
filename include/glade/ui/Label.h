#pragma once

#include "Widget.h"
#include "font/BitmapFont.h"
#include "font/FreetypeFont.h"
#include "layout/CenterLayout.h"

extern Glade::ResourceManager *resource_manager;

class Label : public Widget
{
  private:
    std::string text;
    
  public:
    Label(const std::string &string, std::shared_ptr<Font> &font, float r = 0.0f, float g = 0.0f, float b = 0.0f): Widget()
    {
      Layout::Unique layout(new CenterLayout());
      setLayout(layout);
      
      setFocusable(false);
      /*
      Drawable *debug = new Drawable(Rectangle::INSTANCE, resource_manager->getShaderProgram("texcoord_frames.vertex.glsl", "constant.fragment.glsl"));
      debug->setUniform("uColor", Vector4f(1.0f, 0.0f, 0.0f, 1.0f));
      addDrawable(debug);
      debug->preserveScale = false;
      */
      
      addDrawables(*font->createDrawablesForString(string));
      
      log("SCALE X: %3.3f  Y: %3.3f", font->getLastStringScaleX(), font->getLastStringScaleY());
      getTransform()->getScale()->set(font->getLastStringScaleX(),font->getLastStringScaleY(), 1.0f);
      setTextColor(r, g, b);

      text = string;
    }
  /*
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
    */
    virtual ~Label()
    {
      // TODO delete drawables
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
