#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

/* local */
#include "stateval/Widget.h"

using namespace std;

Widget::Widget(WidgetRenderer &widgetRenderer, const std::string &name) :
  mName(name),
  mWidgetRenderer(&widgetRenderer)
{
  // create local Variable as copy if given
  /*if(value)
  {    
    mValue = value->copy();
  }*/
}

Widget::~Widget()
{
  // TODO: clear container
  //delete mValue;
}

const std::string Widget::getName() const
{
  return mName;
}

void Widget::setProperty(const std::string &name, const Variable &property)
{
  // TODO: check if name element yet exist and then copy value or delete before
  mProperties[name] = property.copy();
   
  /*if(mValue)
  {    
    mValue->copy(&value);
  }
  else
  {
    mValue = value.copy();
  }*/
}

void Widget::updateContent()
{
  // in default implementation Widget has no action
}

void Widget::setWidgetRender(WidgetRenderer &widgetRenderer)
{
  mWidgetRenderer = &widgetRenderer;
}

