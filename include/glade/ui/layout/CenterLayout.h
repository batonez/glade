#pragma once

#include "glade/math/Vector.h"
#include "Layout.h"

class CenterLayout : public Layout
{
  public:
    virtual void calculateTransformsForDirectChildrenOf(Widget* widget)
    {
      rescaleChildrenOf(widget);
      
      Transform* parentTransform = widget->getTransform();
      Widget::Children* children = widget->getChildren();
      
      for (Widget::ChildrenI child = children->begin(); child != children->end(); ++child) {
        (*child)->getTransform()->getPosition()->x = parentTransform->getPosition()->x;
        (*child)->getTransform()->getPosition()->y = parentTransform->getPosition()->y;
      }
    }
};
