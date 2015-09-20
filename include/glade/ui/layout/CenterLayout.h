#pragma once

#include "glade/math/Vector.h"
#include "Layout.h"

class CenterLayout : public Layout
{
	public:
    static CenterLayout INSTANCE;
    
    virtual void calculateTransformsForChildrenOf(Widget* widget)
    {
      log("Calculating Center Layout");
      rescaleChildrenOf(widget);
      
      Transform* parentTransform = widget->getTransform();
      Widget::Widgets* children = widget->getChildren();
      
      for (Widget::WidgetsI child = children->begin(); child != children->end(); ++child) {
        (*child)->getTransform()->getPosition()->x = parentTransform->getPosition()->x;
        (*child)->getTransform()->getPosition()->y = parentTransform->getPosition()->y;
        
        (*child)->getLayout()->calculateTransformsForChildrenOf(*child);
      }
      
      log("Calculating Center Layout finished");
    }
};
