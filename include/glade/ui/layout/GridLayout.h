#pragma once

#include <glade/math/Transform.h>
#include <glade/math/Vector.h>
#include <glade/ui/Widget.h>

class GridLayout : public Layout
{
  private:
    int rowLength;
    
  public:  
    GridLayout(int rowLength = 0):
      rowLength(rowLength)
    {
    }
    
    virtual void calculateTransformsForDirectChildrenOf(Widget *widget)
    {
      rescaleChildrenOf(widget);
      
      float cellWidth, cellHeight;
      cellWidth = cellHeight = 2 * widget->getTransform()->getScale()->x / rowLength;
      
      Transform *parentTransform = widget->getTransform();
      Vector4f *padding = getPadding();
      
      float  homeX   = parentTransform->getPosition()->x + padding->w,
          currentX = homeX,
          currentY = parentTransform->getPosition()->y + padding->x,
          currentZ = parentTransform->getPosition()->z;
      
      int numberInARow = 0;
      
      Widget::ChildrenI child = widget->getChildren()->begin();
      
      while (child != widget->getChildren()->end()) {
        ++numberInARow;
        Transform::SharedVector childScale = (*child)->getTransform()->getScale();
        (*child)->getTransform()->getPosition()->x = currentX - parentTransform->getScale()->x + childScale->x;
        (*child)->getTransform()->getPosition()->y = currentY - parentTransform->getScale()->y + childScale->y;
        (*child)->getTransform()->getPosition()->z = currentZ;
        
        if (numberInARow >= rowLength) {
          currentY += cellHeight;
          currentX = homeX;
          numberInARow = 0;
        } else {
          currentX += cellWidth;
        }
        
        ++child;
      }
    }
    
    /**
     * 
     * @param rowLength Number of cells in the row
     */
    void setRowLength(int rowLength)
    {
      this->rowLength = rowLength;
    }
};
