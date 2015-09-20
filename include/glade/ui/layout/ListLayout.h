#pragma once

#include "glade/math/Vector.h"
#include "Layout.h"

class ListLayout : public Layout
{
	public:
    static ListLayout INSTANCE;
    
    virtual void calculateTransformsForChildrenOf(Widget* widget)
    {
      rescaleChildrenOf(widget);
      
      Transform* parentTransform = widget->getTransform();
      Vector3f* parentPosition = parentTransform->getPositionP();
      
      float
        currentX = parentPosition->x,
        currentY = parentPosition->y,
        currentZ = parentPosition->z;
      
      Widget::Widgets* children = widget->getChildren();
      
      for (Widget::WidgetsI child = children->begin(); child != children->end(); ++child) {
        Transform* childTransform = (*child)->getTransform();
        Vector3f* childScale = childTransform->getScaleP();
        Vector3f* parentScale = parentTransform->getScaleP();
        
        log("CHILD: %f, %f, %f", childScale->x, childScale->y, childScale->z);
        log("PARENT: %f, %f, %f", parentScale->x, parentScale->y, parentScale->z);
        
        float horizontalAlignOffset = 0;
        float verticalAlignOffset = 0;
        
        switch (horizontalAlign) {
          case LEFT:
            horizontalAlignOffset = -parentScale->x + childScale->x  + padding.w;
            break;
          case RIGHT:
            horizontalAlignOffset = parentScale->x - childScale->x  - padding.y;
            break;
        }
        
        switch (verticalAlign) {
          case TOP:
            verticalAlignOffset = -parentScale->y + childScale->y + padding.x;
            break;
          case BOTTOM:
            verticalAlignOffset = parentScale->y - childScale->y + padding.z;
            break;
        }
        
        Vector3f* childPosition = childTransform->getPositionP();
        
        childPosition->x = currentX + horizontalAlignOffset;
        childPosition->y = currentY + verticalAlignOffset;
        childPosition->z = currentZ;
        
        (*child)->getLayout()->calculateTransformsForChildrenOf(*child);
        
        currentY += 2 * childScale->y;
      }
    }
};
