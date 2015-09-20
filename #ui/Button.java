package ru.hatchery.glade.ui;

import ru.hatchery.glade.ui.font.BitmapFont;
import ru.hatchery.glade.ui.layout.Layout;

public class Button extends Widget
{
	private Label label;
	
	public Button()
	{
		this(null, null, new Layout());
	}
	
	public Button(Layout layout)
	{
		this(null, null, layout);
	}
	
	public Button(String text, BitmapFont font)
	{
		this(text, font, new Layout());
	}
	
	public Button(String text, BitmapFont font, Layout layout)
	{
		super(layout);
		setFocusable(true);
		
		if (font != null) {
			label = font.getWidgetForString(text);
			
			getTransform().getScale().x = label.getTransform().getScale().x;
			getTransform().getScale().y = label.getTransform().getScale().y;
			label.setParent(this);
		}
	}
	
	public Label getLabel()
	{
		return label;
	}
}
