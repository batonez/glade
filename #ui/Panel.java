package ru.hatchery.glade.ui;

import ru.hatchery.glade.ui.layout.Layout;

public class Panel extends Widget
{
	public Panel()
	{
		this(new Layout());
	}
	
	public Panel(Layout layout)
	{
		super(layout);
		setFocusable(false);
	}
}
