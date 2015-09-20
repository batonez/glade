#pragma once

#include <glade/math/Transform.h>
#include <glade/math/Vector.h>
#include <glade/ui/Widget.h>

class GridLayout : public Layout
{
  public:
    static GridLayout INSTANCE;
    
	private:
    int rowLength;
	
	protected:
    GridLayout(int rowLength = 0):
      rowLength(rowLength)
    {
    }
    
  public:  
    virtual void calculateTransformsForChildrenOf(Widget *widget)
    {
      log("Calculating Grid Layout");
      rescaleChildrenOf(widget);
      
      float cellWidth, cellHeight;
      cellWidth = cellHeight = 2 * widget->getTransform()->getScale()->x / rowLength;
      
      Transform *parentTransform = widget->getTransform();
      Vector4f *padding = getPadding();
      
      float	homeX	 = parentTransform->getPosition()->x + padding->w,
          currentX = homeX,
          currentY = parentTransform->getPosition()->y + padding->x,
          currentZ = parentTransform->getPosition()->z;
      
      int numberInARow = 0;
      
      Widget::WidgetsI child = widget->getChildren()->begin();
      
      while (child != widget->getChildren()->end()) {
        ++numberInARow;
        Transform::SharedVector childScale = (*child)->getTransform()->getScale();
        (*child)->getTransform()->getPosition()->x = currentX - parentTransform->getScale()->x + childScale->x;
        (*child)->getTransform()->getPosition()->y = currentY - parentTransform->getScale()->y + childScale->y;
        (*child)->getTransform()->getPosition()->z = currentZ;
        
        (*child)->getLayout()->calculateTransformsForChildrenOf(*child);
        
        if (numberInARow >= rowLength) {
          currentY += cellHeight;
          currentX = homeX;
          numberInARow = 0;
        } else {
          currentX += cellWidth;
        }
        
        ++child;
      }
      
      log("Calculating Grid Layout finished");
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
