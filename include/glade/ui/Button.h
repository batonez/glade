#pragma once

#include "glade/ui/Widget.h"
#include "glade/ui/Label.h"
#include "glade/ui/font/BitmapFont.h"
#include "glade/ui/layout/CenterLayout.h"

class Button : public Widget
{
	private:
    Label* label;
	
	public:
    Button() : Button(NULL, NULL)
    {
    }
      
    Button(std::string *text, BitmapFont *font) : Widget(CenterLayout::INSTANCE),
      label(NULL)
    {
      setFocusable(true);
      
      if (font != NULL) {
        label = new Label(*text, *font);
        
        getTransform()->getScale()->x = label->getTransform()->getScale()->x;
        getTransform()->getScale()->y = label->getTransform()->getScale()->y;
        addChild(label);
      }
    }
    
    virtual ~Button()
    {
      if (label) {
        delete label;
      }
    }
  
    Label* getLabel()
    {
      return label;
    }
};
