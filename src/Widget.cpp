#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

/* local */
#include "stateval/private/Widget.h"

using namespace std;

Widget::Widget (const std::string &name, const std::string &variable) :
  mLogger ("stateval.Widget"),
  mName (name),
  mVariable (variable)
{
}

const std::string Widget::getName () const
{
  return mName;
}

const std::string Widget::getVariable () const
{
  return mVariable;
}
