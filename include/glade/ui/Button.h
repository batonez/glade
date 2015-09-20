#pragma once

#include "glade/ui/Widget.h"
#include "glade/ui/Label.h"
#include "glade/ui/font/BitmapFont.h"
#include "glade/ui/layout/CenterLayout.h"

class Button : public Widget
{
	private:
    Label* label = NULL;
	
	public:
    Button() : Button(NULL, NULL)
    {
    }
      
    Button(std::string *text, BitmapFont *font) : Widget(CenterLayout::INSTANCE)
    {
      setFocusable(true);
      
      if (font != NULL) {
        label = new Label(*text, *font);
        
        getTransform()->getScaleP()->x = label->getTransform()->getScaleP()->x;
        getTransform()->getScaleP()->y = label->getTransform()->getScaleP()->y;
        label->setParent(this);
      }
    }
    
    ~Button()
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
