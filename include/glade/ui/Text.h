#pragma once

#include "Widget.h"
#include "font/BitmapFont.h"
#include "font/FreetypeFont.h"
#include "layout/CenterLayout.h"

extern Glade::ResourceManager *resource_manager;

class Text: public Widget
{
  private:
    std::string text;
    std::shared_ptr font;
    
  public:
    Text(const std::string &string, std::shared_ptr<Font> &font_param,
      float r = 0.0f, float g = 0.0f, float b = 0.0f
      float scaleX = 1.0f, float scaleY = 1.0f):
        Widget(),
        font(font_param),
        text(string)
    {
      assert(font != nullptr);
      
      Layout::Unique layout(new CenterLayout());
      setLayout(layout);
      
      setFocusable(false);
      /*
      Drawable *debug = new Drawable(Rectangle::INSTANCE, resource_manager->getShaderProgram("texcoord_frames.vertex.glsl", "constant.fragment.glsl"));
      debug->setUniform("uColor", Vector4f(1.0f, 0.0f, 0.0f, 1.0f));
      addDrawable(debug);
      debug->preserveScale = false;
      */
      
      setTextColor(r, g, b);
      
      setScale(scaleX, scaleY);
    }
    
    virtual ~Text()
    {
      // FIXME delete drawables
    }
    
    void setScale(float scaleX, float scaleY)
    {
      getTransform()->setScale(scaleX, scaleY, 1.0f);
      addDrawables(*font->createDrawablesWordWrapped(string, scaleX));
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
