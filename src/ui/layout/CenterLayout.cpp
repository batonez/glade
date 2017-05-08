#include <glade/ui/layout/CenterLayout.h>
#include <glade/ui/Widget.h>

void CenterLayout::calculateTransformsForDirectChildrenOf(Widget* widget) const
{
  rescaleChildrenOf(widget);
  
  Transform* parentTransform = widget->getTransform();
  Widget::Children* children = widget->getChildren();
  
  for (Widget::ChildrenI child = children->begin(); child != children->end(); ++child) {
    (*child)->getTransform()->getPosition()->x = parentTransform->getPosition()->x;
    (*child)->getTransform()->getPosition()->y = parentTransform->getPosition()->y;
  }
}
