#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

/* local */
#include "stateval/Widget.h"

using namespace std;

Widget::Widget(const std::string &name, const Variable *value) :
  mName(name),
  mValue(NULL)
{
  // create local Variable as copy if given
  if(value)
  {    
    mValue = value->copy();
  }
}

Widget::~Widget()
{
  delete mValue;
}

const std::string Widget::getName() const
{
  return mName;
}

void Widget::setValue(const Variable &value)
{
  if(mValue)
  {    
    mValue->copy(&value);
  }
  else
  {
    mValue = value.copy();
  }
}

void Widget::updateContent()
{
  // in default implementation Widget has no action
}


