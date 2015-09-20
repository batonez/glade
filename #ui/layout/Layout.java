package ru.hatchery.glade.ui.layout;

import ru.hatchery.glade.exception.GladeRuntimeException;
import ru.hatchery.glade.model.Transform;
import ru.hatchery.glade.model.Vector2f;
import ru.hatchery.glade.model.Vector4f;
import ru.hatchery.glade.ui.Widget;

public class Layout
{
	protected Vector2f minContentPosition = new Vector2f();
	protected Vector2f maxContentPosition = new Vector2f();
	protected Vector4f padding = new Vector4f();
	protected Vector4f margin  = new Vector4f();
	protected HorizontalAlign horizontalAlign = HorizontalAlign.LEFT;
	protected VerticalAlign verticalAlign = VerticalAlign.TOP;
	protected float horizontalSizePercent;
	protected float verticalSizePercent;
	protected SizeType horizontalSizeType = SizeType.MANUAL;
	protected SizeType verticalSizeType = SizeType.MANUAL;
	
	public void setVerticalAlign(VerticalAlign verticalAlign)
	{
		this.verticalAlign = verticalAlign;
	}
	
	public void setHorizontalAlign(HorizontalAlign horizontalAlign)
	{
		this.horizontalAlign = horizontalAlign;
	}
	
	public Vector4f getPadding()
	{
		return padding;
	}
	
	public void setPadding(float top, float right, float bottom, float left)
	{
		padding.x = top;
		padding.y = right;
		padding.z = bottom;
		padding.w = left;
	}
	
	public Vector4f getMargin()
	{
		return margin;
	}
	
	public void setMargin(float top, float right, float bottom, float left)
	{
		margin.x = top;
		margin.y = right;
		margin.z = bottom;
		margin.w = left;
	}
	
	public float getHorizontalSizePercent()
	{
		return horizontalSizePercent;
	}
	
	public void setHorizontalSizePercent(Float horizontalSize)
	{
		this.horizontalSizePercent = horizontalSize;
	}
	
	public float getVerticalSizePercent()
	{
		return verticalSizePercent;
	}
	
	public void setVerticalSizePercent(Float verticalSize)
	{
		this.verticalSizePercent = verticalSize;
	}
	
	public void setHorizontalSizeType(SizeType sizeType)
	{
		if (sizeType == SizeType.PERCENT_OF_SELF_WIDTH) {
			throw new GladeRuntimeException("Horizontal size cannot be specified as percents of itself.");
		}
		
		if (sizeType == SizeType.PERCENT_OF_SELF_HEIGHT && verticalSizeType == SizeType.PERCENT_OF_SELF_WIDTH) {
			throw new GladeRuntimeException("Cannot specify horizontal size as percents of self height, becouse height is already specified as percents of this horizontal size.");
		}
		
		this.horizontalSizeType = sizeType;
	}
	
	public SizeType getHorizontalSizeType()
	{
		return horizontalSizeType;
	}
	
	public void setVerticalSizeType(SizeType sizeType)
	{
		if (sizeType == SizeType.PERCENT_OF_SELF_HEIGHT) {
			throw new GladeRuntimeException("Vertical size cannot be specified as percents of itself.");
		}
		
		if (sizeType == SizeType.PERCENT_OF_SELF_WIDTH && verticalSizeType == SizeType.PERCENT_OF_SELF_HEIGHT) {
			throw new GladeRuntimeException("Cannot specify vertical size as percents of self width, becouse width is already specified as percents of this vertical size.");
		}
		
		this.verticalSizeType = sizeType;
	}
	
	public SizeType getVerticalSizeType()
	{
		return verticalSizeType;
	}
	
	protected void rescaleChildrenOf(Widget widget)
	{
		for (Widget child: widget.getChildren()) {
			if (!rescaleX(child, widget)) {
				rescaleY(child, widget);
			}
		}
	}
	
	/** Возвращает true если в результате были расчитаны оба размера X и Y. Если был расчитан только X - возвращает false */
	protected boolean rescaleX(Widget widget, Widget hisParent)
	{
		Float horizontalSize = widget.getLayout().getHorizontalSizePercent();
		boolean result = false;
		
		switch (widget.getLayout().getHorizontalSizeType()) {
			case PERCENT_OF_PARENT:
				widget.getTransform().getScale().x = horizontalSize * hisParent.getTransform().getScale().x - ((padding.y + padding.w) / 2);
				break;
			case PERCENT_OF_SELF_HEIGHT:
				rescaleY(widget, hisParent);
				widget.getTransform().getScale().x = horizontalSize * widget.getTransform().getScale().y;
				result = true;
				break;
			default:
				break;
		}
		
		return result;
	}
	
	/** Возвращает true если в результате были расчитаны оба размера X и Y. Если был расчитан только Y - возвращает false */
	protected boolean rescaleY(Widget widget, Widget hisParent)
	{
		Float verticalSize = widget.getLayout().getVerticalSizePercent();
		boolean result = false;
		
		switch (widget.getLayout().verticalSizeType) {
			case PERCENT_OF_PARENT:
				widget.getTransform().getScale().y = verticalSize * hisParent.getTransform().getScale().y - ((padding.x + padding.z) / 2);
				break;
			case PERCENT_OF_SELF_WIDTH:
				rescaleX(widget, hisParent);
				widget.getTransform().getScale().y = verticalSize * widget.getTransform().getScale().x;
				result = true;
				break;
			default:
				break;
		}
		
		return result;
	}
	
	public void calculateTransformsForChildrenOf(Widget widget)
	{
		rescaleChildrenOf(widget);
		
		Transform parentTransform = widget.getTransform();
		
		for (Widget child: widget.getChildren()) {
			child.getTransform().getPosition().x = parentTransform.getPosition().x;
			child.getTransform().getPosition().y = parentTransform.getPosition().y;
			
			child.getLayout().calculateTransformsForChildrenOf(child);
		}
	}
	
	public enum SizeType
	{
		MANUAL,
		PERCENT_OF_PARENT,
		PERCENT_OF_SELF_WIDTH,
		PERCENT_OF_SELF_HEIGHT,
		WRAP_CONTENT
	}
	
	public enum HorizontalAlign
	{
		LEFT, RIGHT, CENTER;
	}
	
	public enum VerticalAlign
	{
		TOP, BOTTOM, CENTER;
	}
}
