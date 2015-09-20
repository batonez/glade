#pragma once

#include "glade/math/Vector.h"
#include "Layout.h"
#include "glade/ui/Widget.h"

class SequenceLayout : public Layout
{
	public:
    static SequenceLayout INSTANCE;
    
    virtual void calculateTransformsForChildrenOf(Widget* widget)
    {
      rescaleChildrenOf(widget);
      
      Transform* parentTransform = widget->getTransform();
      
      float
        currentX = parentTransform->getPositionP()->x + padding.w,
        currentY = parentTransform->getPositionP()->y + padding.x,
        currentZ = parentTransform->getPositionP()->z;
      
      Widget::Widgets* children = widget->getChildren();
      
      for (Widget::WidgetsI child = children->begin(); child != children->end(); ++child) {
        Transform* childTransform = (*child)->getTransform();
        Vector3f* childScale = childTransform->getScaleP();
        Vector3f* childPosition = childTransform->getPositionP();
        Vector3f* parentScale = parentTransform->getScaleP();
        
        childPosition->x = currentX - parentScale->x + childScale->x;
        childPosition->y = currentY - parentScale->y + childScale->y;
        childPosition->z = currentZ;
        
        (*child)->getLayout()->calculateTransformsForChildrenOf(*child);
        
        currentX += 2 * childScale->x;
      }
    }
};
