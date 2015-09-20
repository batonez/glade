package ru.hatchery.glade.ui;

import ru.hatchery.glade.model.Vector3f;
import ru.hatchery.glade.render.Drawable;
import ru.hatchery.glade.ui.font.BitmapFont;
import ru.hatchery.glade.ui.layout.SequenceLayout;

public class Label extends Widget
{
	public static Label forString(String string, BitmapFont font)
	{
		return font.getWidgetForString(string);
	}
	
	public Label()
	{
		super(new SequenceLayout());
		setFocusable(false);
	}
	
	@Override
	public void addDrawable(Drawable view)
	{
		super.addDrawable(view);
		view.preserveScale = true;
	}
	
	public void setTextColor(Vector3f color)
	{
		for (Drawable drawable: getDrawables()) {
			drawable.replaceOriginalColor = true;
			drawable.setConstantColor(color);
		}
	}
}
