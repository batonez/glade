package ru.hatchery.glade.ui.layout;

import ru.hatchery.glade.model.Transform;
import ru.hatchery.glade.model.Vector3f;
import ru.hatchery.glade.model.Vector4f;
import ru.hatchery.glade.ui.Widget;

public class ListLayout extends Layout
{
	@Override
	public void calculateTransformsForChildrenOf(Widget widget)
	{
		rescaleChildrenOf(widget);
		
		Transform parentTransform = widget.getTransform();
		Vector4f padding = getPadding();
		
		float	currentX = parentTransform.getPosition().x,
				currentY = parentTransform.getPosition().y,
				currentZ = parentTransform.getPosition().z;
		
		for (Widget child: widget.getChildren()) {
			Vector3f childScale = child.getTransform().getScale();
			
			float horizontalAlignOffset;
			float verticalAlignOffset;
			
			switch (horizontalAlign) {
				case LEFT:
					horizontalAlignOffset = -parentTransform.getScale().x + childScale.x  + padding.w;
					break;
				case RIGHT:
					horizontalAlignOffset = parentTransform.getScale().x - childScale.x  - padding.y;
					break;
				default:
					horizontalAlignOffset = 0;
					break;
			}
			
			switch (verticalAlign) {
				case TOP:
					verticalAlignOffset = -parentTransform.getScale().y + childScale.y + padding.x;
					break;
				case BOTTOM:
					verticalAlignOffset = parentTransform.getScale().y - childScale.y + padding.z;
					break;
				default:
					verticalAlignOffset = 0;
					break;
			}
			
			child.getTransform().getPosition().x = currentX + horizontalAlignOffset;
			child.getTransform().getPosition().y = currentY + verticalAlignOffset;
			child.getTransform().getPosition().z = currentZ;
			
			child.getLayout().calculateTransformsForChildrenOf(child);
			
			currentY += 2 * child.getTransform().getScale().y;
		}
	}
}
