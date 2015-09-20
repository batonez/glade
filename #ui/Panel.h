#pragma once

#include "Widget.h"

class Layout;

class Panel : public Widget
{
	public:  
    Panel(Layout* layout) : Widget(layout)
    {
      setFocusable(false);
    }
  
  /*
  Panel()
	{
		this(new Layout());
	}*/
};
