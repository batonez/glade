package ru.hatchery.glade.ui.layout;

import ru.hatchery.glade.model.Transform;
import ru.hatchery.glade.model.Vector3f;
import ru.hatchery.glade.model.Vector4f;
import ru.hatchery.glade.ui.Widget;

public class SequenceLayout extends Layout
{
	@Override
	public void calculateTransformsForChildrenOf(Widget widget)
	{
		rescaleChildrenOf(widget);
		
		Transform parentTransform = widget.getTransform();
		Vector4f padding = getPadding();
		
		float	currentX = parentTransform.getPosition().x + padding.w,
				currentY = parentTransform.getPosition().y + padding.x,
				currentZ = parentTransform.getPosition().z;
		
		for (Widget child: widget.getChildren()) {
			Vector3f childScale = child.getTransform().getScale();
			child.getTransform().getPosition().x = currentX - parentTransform.getScale().x + childScale.x;
			child.getTransform().getPosition().y = currentY - parentTransform.getScale().y + childScale.y;
			child.getTransform().getPosition().z = currentZ;
			
			child.getLayout().calculateTransformsForChildrenOf(child);
			
			currentX += 2 * child.getTransform().getScale().x;
		}
	}
}
