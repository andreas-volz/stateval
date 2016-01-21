#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

/* local */
#include "stateval/private/Widget.h"

using namespace std;

Widget::Widget(const std::string &name, const Variable *value) :
  mLogger("stateval.Widget"),
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

/*const std::string Widget::getVariable() const
{
  return mValue;
}*/

void Widget::setValue(const Variable &val)
{
  // in default implementation Widget has no action
}

void Widget::updateContent()
{
  // in default implementation Widget has no action
}


