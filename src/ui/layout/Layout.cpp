#include <assert.h>
#include <glade/ui/layout/Layout.h>
#include <glade/ui/Widget.h>

void Layout::rescaleChildrenOf(Widget* widget) const
{
  Widget::Children* children = widget->getChildren();
  
  for (Widget::ChildrenI child = children->begin(); child != children->end(); ++child) {
    if (!rescaleX(*child, widget)) {
      rescaleY(*child, widget);
    }
  }
}

/** Возвращает true если в результате были расчитаны оба размера X и Y. Если был расчитан только X - возвращает false */
bool Layout::rescaleX(Widget* widget, Widget* hisParent) const
{
  assert(widget->getLayout() != nullptr);

  float horizontalSize = widget->getLayout()->get()->getHorizontalSizePercent();
  bool result = false;
  
  switch (widget->getLayout()->get()->getHorizontalSizeType()) {
    case PERCENT_OF_PARENT:
      widget->getTransform()->getScale()->x = horizontalSize * hisParent->getTransform()->getScale()->x - ((padding.y + padding.w) / 2);
      break;
    case PERCENT_OF_SELF_HEIGHT:
      rescaleY(widget, hisParent);
      widget->getTransform()->getScale()->x = horizontalSize * widget->getTransform()->getScale()->y;
      result = true;
      break;
    default:
      break;
  }
  
  return result;
}

/** Возвращает true если в результате были расчитаны оба размера X и Y. Если был расчитан только Y - возвращает false */
bool Layout::rescaleY(Widget* widget, Widget* hisParent) const
{
  assert(widget->getLayout() != nullptr);

  float verticalSize = widget->getLayout()->get()->getVerticalSizePercent();
  bool result = false;
  
  switch (widget->getLayout()->get()->verticalSizeType) {
    case PERCENT_OF_PARENT:
      widget->getTransform()->getScale()->y = verticalSize * hisParent->getTransform()->getScale()->y - ((padding.x + padding.z) / 2);
      break;
    case PERCENT_OF_SELF_WIDTH:
      rescaleX(widget, hisParent);
      widget->getTransform()->getScale()->y = verticalSize * widget->getTransform()->getScale()->x;
      result = true;
      break;
    default:
      break;
  }
  
  return result;
}
