#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

/* Project */
#include "stateval/Variable.h"

AbstractVariable::AbstractVariable (Type type) :
  mType (type)
{ 
}

AbstractVariable::Type AbstractVariable::getType () const
{
  return mType;  
}

Bool::Bool (bool b) :
  AbstractVariable (TYPE_BOOL),
  mValue (b)
{
  
}

bool Bool::compare (AbstractVariable *var) const
{
  cout << "this:Tyoe: " << getType () << endl;
  cout << "var:Type: " << var->getType () << endl;

  bool ret = (static_cast <Bool*> (var))->mValue == mValue;
  
  return ret;
}

