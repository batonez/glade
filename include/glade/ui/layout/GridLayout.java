package ru.hatchery.glade.ui.layout;

import ru.hatchery.glade.model.Transform;
import ru.hatchery.glade.model.Vector3f;
import ru.hatchery.glade.model.Vector4f;
import ru.hatchery.glade.ui.Widget;

public class GridLayout extends Layout
{
	private int rowLength;
//	private float cellWidth;
//	private float cellHeight;
	
	public GridLayout(int rowLength)
	{
		setRowLength(rowLength);
	}
	
	@Override
	public void calculateTransformsForChildrenOf(Widget widget)
	{
		rescaleChildrenOf(widget);
		
		float cellWidth, cellHeight;
		cellWidth = cellHeight = 2 * widget.getTransform().getScale()->x / rowLength;
		
		Transform parentTransform = widget.getTransform();
		Vector4f padding = getPadding();
		
		float	homeX	 = parentTransform.getPosition()->x + padding.w,
				currentX = homeX,
				currentY = parentTransform.getPosition()->y + padding.x,
				currentZ = parentTransform.getPosition()->z;
		
		int numberInARow = 0;
		
		for (Widget child: widget.getChildren()) {
			++ numberInARow;
			Vector3f childScale = child.getTransform().getScale();
			child.getTransform().getPosition()->x = currentX - parentTransform.getScale()->x + childScale.x;
			child.getTransform().getPosition()->y = currentY - parentTransform.getScale()->y + childScale.y;
			child.getTransform().getPosition()->z = currentZ;
			
			child.getLayout().calculateTransformsForChildrenOf(child);
			
			if (numberInARow >= rowLength) {
				currentY += cellHeight;
				currentX = homeX;
				numberInARow = 0;
			} else {
				currentX += cellWidth;
			}
		}
	}
	
	/**
	 * 
	 * @param rowLength Number of cells in the row
	 */
	public void setRowLength(int rowLength)
	{
		this.rowLength = rowLength;
		
	}
}
