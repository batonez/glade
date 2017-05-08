#pragma once

#include "Layout.h"

class CenterLayout : public Layout
{
  public:
    virtual void calculateTransformsForDirectChildrenOf(Widget* widget) const;
};
