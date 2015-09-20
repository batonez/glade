#pragma once

#include "Widget.h"
#include "layout/CenterLayout.h"

class Layout;

class Panel : public Widget
{
	public:  
    Panel(Layout::Unique &layout) : Widget(layout)
    {
      setFocusable(false);
    }
  
    Panel() : Panel(Layout::Unique(new CenterLayout()))
    {
    }
};
