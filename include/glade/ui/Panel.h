#pragma once

#include "Widget.h"
#include "layout/CenterLayout.h"

class Layout;

class Panel : public Widget
{
  public:  
    Panel(Layout::Unique &layout): Widget(layout)
    {
      setFocusable(false);
    }
  
    Panel(): Widget()
    {
      Layout::Unique layout(new CenterLayout());
      setLayout(layout);
    }
};
