#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

/* local */
#include "stateval/Variable.h"

/* STD */
#include <cassert>

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

bool Bool::equals (AbstractVariable *var) const
{
  cout << "equals this:Type: " << getType () << endl;
  cout << "var:Type: " << var->getType () << endl;
  assert (getType () == var->getType ());
  
  bool ret = (static_cast <Bool*> (var))->mValue == mValue;
  
  return ret;
}

bool Bool::assign (AbstractVariable *var)
{
  cout << "assign this:Type: " << getType () << endl;
  cout << "var:Type: " << var->getType () << endl;
  assert (getType () == var->getType ());

  mValue = (static_cast <Bool*> (var))->mValue;
}

String::String (const std::string &s) :
  AbstractVariable (TYPE_STRING),
  mValue (s)
{
  
}

bool String::equals (AbstractVariable *var) const
{
  cout << "equals this:Type: " << getType () << endl;
  cout << "var:Type: " << var->getType () << endl;
  assert (getType () == var->getType ());
  
  bool ret = (static_cast <String*> (var))->mValue == mValue;
  
  return ret;
}

bool String::assign (AbstractVariable *var)
{
  cout << "assign this:Type: " << getType () << endl;
  cout << "var:Type: " << var->getType () << endl;
  assert (getType () == var->getType ());

  mValue = (static_cast <String*> (var))->mValue;
}
