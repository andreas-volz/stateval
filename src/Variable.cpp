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

void Bool::assign (AbstractVariable *var)
{
  cout << "assign this:Type: " << getType () << endl;
  cout << "var:Type: " << var->getType () << endl;
  assert (getType () == var->getType ());

  mValue = (static_cast <Bool*> (var))->mValue;
}

bool Bool::getData () const
{
  return mValue;
}

Float::Float (float f) :
  AbstractVariable (TYPE_FLOAT),
  mValue (f)
{
  
}

bool Float::equals (AbstractVariable *var) const
{
  cout << "equals this:Type: " << getType () << endl;
  cout << "var:Type: " << var->getType () << endl;
  assert (getType () == var->getType ());
  
  float ret = (static_cast <Float*> (var))->mValue == mValue;
  
  return ret;
}

void Float::assign (AbstractVariable *var)
{
  cout << "assign this:Type: " << getType () << endl;
  cout << "var:Type: " << var->getType () << endl;
  assert (getType () == var->getType ());

  mValue = (static_cast <Float*> (var))->mValue;
}

float Float::getData () const
{
  return mValue;
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

void String::assign (AbstractVariable *var)
{
  cout << "assign this:Type: " << getType () << endl;
  cout << "var:Type: " << var->getType () << endl;
  assert (getType () == var->getType ());

  mValue = (static_cast <String*> (var))->mValue;
}

std::string String::getData () const
{
  return mValue;
}

Struct::Struct (const std::string &s) :
  AbstractVariable (TYPE_STRUCT)
{

}

bool Struct::equals (AbstractVariable *var) const
{
  cout << "equals this:Type: " << getType () << endl;
  cout << "var:Type: " << var->getType () << endl;
  assert (getType () == var->getType ());

  // TODO: implement
  bool ret = false;//(static_cast <String*> (var))->mValue == mValue;
  
  return ret;
}

void Struct::assign (AbstractVariable *var)
{
  cout << "assign this:Type: " << getType () << endl;
  cout << "var:Type: " << var->getType () << endl;
  assert (getType () == var->getType ());

  // TODO: implement
  //mValue = (static_cast <String*> (var))->mValue;
}

void Struct::add (const std::string &s, AbstractVariable *var)
{
  mValueMap[s] = var;
}

AbstractVariable *Struct::getData (const std::string &s)
{
  cout << "mValueMap size: " << mValueMap.size () << endl;
  return mValueMap[s];
}
