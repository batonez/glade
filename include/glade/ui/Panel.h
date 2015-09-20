#pragma once

#include "Widget.h"
#include "layout/CenterLayout.h"

class Layout;

class Panel : public Widget
{
	public:  
    Panel(Layout &layout) : Widget(layout)
    {
      setFocusable(false);
    }
  
    Panel() : Panel(CenterLayout::INSTANCE)
    {
    }
};
