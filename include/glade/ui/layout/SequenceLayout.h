#pragma once

#include "glade/math/Vector.h"
#include "Layout.h"
#include "glade/ui/Widget.h"
#include "glade/math/Transform.h"

class SequenceLayout : public Layout
{
	public:
    static SequenceLayout INSTANCE;
    
    virtual void calculateTransformsForChildrenOf(Widget* widget)
    {
      log("Calculating Sequence Layout");
      rescaleChildrenOf(widget);
      
      Transform* parentTransform = widget->getTransform();
      
      float
        currentX = parentTransform->getPosition()->x + padding.w,
        currentY = parentTransform->getPosition()->y + padding.x,
        currentZ = parentTransform->getPosition()->z;
      
      Widget::Children* children = widget->getChildren();
      
      for (Widget::ChildrenI child = children->begin(); child != children->end(); ++child) {
        Transform* childTransform = (*child)->getTransform();
        Transform::SharedVector childScale = childTransform->getScale();
        Transform::SharedVector childPosition = childTransform->getPosition();
        Transform::SharedVector parentScale = parentTransform->getScale();
        
        childPosition->x = currentX - parentScale->x + childScale->x;
        childPosition->y = currentY - parentScale->y + childScale->y;
        childPosition->z = currentZ;
        
        (*child)->getLayout()->calculateTransformsForChildrenOf(*child);
        
        currentX += 2 * childScale->x;
      }
      
      log("Calculating Sequence Layout finished");
    }
};
