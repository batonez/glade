#pragma once

#include "glade/math/Vector.h"
#include "Layout.h"

class CenterLayout : public Layout
{
	public:
    static CenterLayout INSTANCE;
    
    virtual void calculateTransformsForChildrenOf(Widget* widget)
    {
      rescaleChildrenOf(widget);
      
      Transform* parentTransform = widget->getTransform();
      Widget::Widgets* children = widget->getChildren();
      
      for (Widget::WidgetsI child = children->begin(); child != children->end(); ++child) {
        (*child)->getTransform()->getPositionP()->x = parentTransform->getPosition().x;
        (*child)->getTransform()->getPositionP()->y = parentTransform->getPosition().y;
        
        (*child)->getLayout()->calculateTransformsForChildrenOf(*child);
      }
    }
};
