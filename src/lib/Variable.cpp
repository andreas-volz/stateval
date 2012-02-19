#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

/* local */
#include "stateval/Variable.h"
#include "MemoryUtil.h"
#include "stateval/private/Logger.h" // TODO: don't use this here

/* STD */
#include <cassert>

// TODO: move into each specific variable class and PImpl
static Logger logger("stateval.Variable");

AbstractVariable::AbstractVariable(Type type) :
  mType(type),
  mNeedsUpdate(false)
{
}

AbstractVariable::Type AbstractVariable::getType() const
{
  return mType;
}

void AbstractVariable::setUpdateFlag(bool flag)
{
  mNeedsUpdate = flag;
}

bool AbstractVariable::needsUpdate ()
{
  return mNeedsUpdate;
}

//////////////////////////

Bool::Bool(bool b) :
  AbstractVariable(TYPE_BOOL),
  mValue(b)
{

}

bool Bool::equals(AbstractVariable *var) const
{
  LOG4CXX_DEBUG(logger, "equals this:Type: " << getType());
  LOG4CXX_DEBUG(logger, "var:Type: " << var->getType());
  assert(getType() == var->getType());

  bool ret = (static_cast <Bool *>(var))->mValue == mValue;

  return ret;
}

void Bool::assign(AbstractVariable *var)
{
  LOG4CXX_DEBUG(logger, "assign this:Type: " << getType());
  LOG4CXX_DEBUG(logger, "var:Type: " << var->getType());
  assert(getType() == var->getType());

  mValue = (static_cast <Bool *>(var))->mValue;
  setUpdateFlag(true);
}

bool Bool::getData() const
{
  return mValue;
}

//////////////////////////

Float::Float(float f) :
  AbstractVariable(TYPE_FLOAT),
  mValue(f)
{

}

bool Float::equals(AbstractVariable *var) const
{
  LOG4CXX_DEBUG(logger, "equals this:Type: " << getType());
  LOG4CXX_DEBUG(logger, "var:Type: " << var->getType());
  assert(getType() == var->getType());

  float ret = (static_cast <Float *>(var))->mValue == mValue;

  return ret;
}

void Float::assign(AbstractVariable *var)
{
  LOG4CXX_DEBUG(logger, "assign this:Type: " << getType());
  LOG4CXX_DEBUG(logger, "var:Type: " << var->getType());
  assert(getType() == var->getType());

  mValue = (static_cast <Float *>(var))->mValue;
  setUpdateFlag(true);
}

float Float::getData() const
{
  return mValue;
}


//////////////////////////

Integer::Integer(int i) :
  AbstractVariable(TYPE_INTEGER),
  mValue(i)
{

}

bool Integer::equals(AbstractVariable *var) const
{
  LOG4CXX_DEBUG(logger, "equals this:Type: " << getType());
  LOG4CXX_DEBUG(logger, "var:Type: " << var->getType());
  assert(getType() == var->getType());

  int ret = (static_cast <Integer *>(var))->mValue == mValue;

  return ret;
}

void Integer::assign(AbstractVariable *var)
{
  LOG4CXX_DEBUG(logger, "assign this:Type: " << getType());
  LOG4CXX_DEBUG(logger, "var:Type: " << var->getType());
  assert(getType() == var->getType());

  mValue = (static_cast <Integer *>(var))->mValue;
  setUpdateFlag(true);
}

int Integer::getData() const
{
  return mValue;
}


//////////////////////////

String::String(const std::string &s) :
  AbstractVariable(TYPE_STRING),
  mValue(s)
{

}

bool String::equals(AbstractVariable *var) const
{
  LOG4CXX_DEBUG(logger, "equals this:Type: " << getType());
  LOG4CXX_DEBUG(logger, "var:Type: " << var->getType());
  assert(getType() == var->getType());

  bool ret = (static_cast <String *>(var))->mValue == mValue;

  return ret;
}

void String::assign(AbstractVariable *var)
{
  LOG4CXX_DEBUG(logger, "assign this:Type: " << getType());
  LOG4CXX_DEBUG(logger, "var:Type: " << var->getType());
  assert(getType() == var->getType());

  mValue = (static_cast <String *>(var))->mValue;
  setUpdateFlag(true);
}

void String::change(const std::string &str)
{
  mValue = str;
  setUpdateFlag(true);
}

std::string String::getData() const
{
  return mValue;
}

//////////////////////////

Struct::Struct() :
  AbstractVariable(TYPE_STRUCT)
{
}

Struct::~Struct()
{
  delete_stl_container(mValueMap);
}

bool Struct::equals(AbstractVariable *var) const
{
  LOG4CXX_DEBUG(logger, "equals this:Type: " << getType());
  LOG4CXX_DEBUG(logger, "var:Type: " << var->getType());
  assert(getType() == var->getType());

  // TODO: implement
  bool ret = false;//(static_cast <String*> (var))->mValue == mValue;
  assert(false);

  return ret;
}

void Struct::assign(AbstractVariable *var)
{
  LOG4CXX_DEBUG(logger, "assign this:Type: " << getType());
  LOG4CXX_DEBUG(logger, "var:Type: " << var->getType());
  assert(getType() == var->getType());

  // TODO: implement
  //mValue = (static_cast <String*> (var))->mValue;
  assert(false);
  setUpdateFlag(true);
}

void Struct::add(const std::string &s, AbstractVariable *var)
{
  mValueMap[s] = var;
  setUpdateFlag(true);
}

AbstractVariable *Struct::getData(const std::string &s)
{
  LOG4CXX_DEBUG(logger, "mValueMap size: " << mValueMap.size());
  return mValueMap[s];
}

Struct::Iterator Struct::begin()
{
  return mValueMap.begin();
}

Struct::Iterator Struct::end()
{
  return mValueMap.end();
}

//////////////////////////

List::List() :
  AbstractVariable(TYPE_LIST)
{
}

List::~List()
{
  clear();
}

bool List::equals(AbstractVariable *var) const
{
  LOG4CXX_DEBUG(logger, "equals this:Type: " << getType());
  LOG4CXX_DEBUG(logger, "var:Type: " << var->getType());
  assert(getType() == var->getType());

  // TODO: implement
  bool ret = false;//(static_cast <String*> (var))->mValue == mValue;
  assert(false);

  return ret;
}

void List::assign(AbstractVariable *var)
{
  LOG4CXX_DEBUG(logger, "assign this:Type: " << getType());
  LOG4CXX_DEBUG(logger, "var:Type: " << var->getType());
  assert(getType() == var->getType());

  // TODO: implement
  //mValue = (static_cast <String*> (var))->mValue;
  assert(false);
  setUpdateFlag(true);
}

void List::pushBack(AbstractVariable *var)
{
  mValueList.push_back(var);
  setUpdateFlag(true);
}

void List::pushFront(AbstractVariable *var)
{
  mValueList.push_front(var);
  setUpdateFlag(true);
}

void List::clear()
{
  delete_stl_container(mValueList);
  setUpdateFlag(true);
}

List::Iterator List::begin()
{
  return mValueList.begin();
}

List::Iterator List::end()
{
  return mValueList.end();
}
