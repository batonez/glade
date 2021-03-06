#pragma once

#include "glade/math/Vector.h"
#include "Layout.h"
#include "glade/math/Transform.h"

class ListLayout : public Layout
{
  public:
    virtual void calculateTransformsForDirectChildrenOf(Widget* widget) const
    {
      rescaleChildrenOf(widget);
      
      Transform* parentTransform = widget->getTransform();
      Transform::SharedVector parentPosition = parentTransform->getPosition();
      
      float
        currentX = parentPosition->x,
        currentY = parentPosition->y,
        currentZ = parentPosition->z;
      
      Widget::Children* children = widget->getChildren();
      
      for (Widget::ChildrenI child = children->begin(); child != children->end(); ++child) {
        Transform* childTransform = (*child)->getTransform();
        Transform::SharedVector childScale = childTransform->getScale();
        Transform::SharedVector parentScale = parentTransform->getScale();
        
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
        
        Transform::SharedVector childPosition = childTransform->getPosition();
        
        childPosition->x = currentX + horizontalAlignOffset;
        childPosition->y = currentY + verticalAlignOffset;
        childPosition->z = currentZ;
        
        currentY += 2 * childScale->y;
      }
    }
};
