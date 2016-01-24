#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

/* local */
#include "stateval/Variable.h"
#include "MemoryUtil.h"
#include "stateval/private/Logger.h" // TODO: don't use this here

/* STD */
#include <cassert>
#include <algorithm>

// TODO: move logger into each specific variable class and PImpl
// TODO: throw exceptions (e.g. for wrong types)
static Logger logger("stateval.Variable");

Variable::Variable(Type type) :
  mType(type),
  mNeedsUpdate(false)
{
}

Variable::Type Variable::getType() const
{
  return mType;
}

void Variable::setUpdateFlag(bool flag)
{
  mNeedsUpdate = flag;
}

bool Variable::needsUpdate ()
{
  return mNeedsUpdate;
}

//////////////////////////

Bool::Bool() :
  Variable(TYPE_BOOL),
  mValue(false)
{
}

Bool::Bool(bool b) :
  Variable(TYPE_BOOL),
  mValue(b)
{
}

bool Bool::equals(const Variable *var) const
{
  LOG4CXX_DEBUG(logger, "equals this:Type: " << getType());
  LOG4CXX_DEBUG(logger, "var:Type: " << var->getType());
  assert(getType() == var->getType());

  bool ret = (dynamic_cast <const Bool*>(var))->mValue == mValue;

  return ret;
}

void Bool::copy(const Variable *var)
{
  LOG4CXX_DEBUG(logger, "copy this:Type: " << getType());
  LOG4CXX_DEBUG(logger, "var:Type: " << var->getType());
  assert(getType() == var->getType());

  mValue = (dynamic_cast <const Bool*>(var))->mValue;
  setUpdateFlag(true);
}

Variable *Bool::copy() const
{
  return new Bool(mValue);
}

bool Bool::getData() const
{
  return mValue;
}

//////////////////////////

Double::Double() :
  Variable(TYPE_DOUBLE),
  mValue(0.0)
{
}

Double::Double(double d) :
  Variable(TYPE_DOUBLE),
  mValue(d)
{
}

bool Double::equals(const Variable *var) const
{
  LOG4CXX_DEBUG(logger, "equals this:Type: " << getType());
  LOG4CXX_DEBUG(logger, "var:Type: " << var->getType());
  assert(getType() == var->getType());

  double ret = (dynamic_cast <const Double*>(var))->mValue == mValue;

  return ret;
}

void Double::copy(const Variable *var)
{
  LOG4CXX_DEBUG(logger, "copy this:Type: " << getType());
  LOG4CXX_DEBUG(logger, "var:Type: " << var->getType());
  assert(getType() == var->getType());

  mValue = (dynamic_cast <const Double*>(var))->mValue;
  setUpdateFlag(true);
}

Variable *Double::copy() const
{
  return new Double(mValue);
}

double Double::getData() const
{
  return mValue;
}

Double Double::operator = (const Double& d)
{
  mValue = d.mValue;
  return *this;
}


//////////////////////////

Integer::Integer() :
  Variable(TYPE_INTEGER),
  mValue(0)
{
}

Integer::Integer(int i) :
  Variable(TYPE_INTEGER),
  mValue(i)
{

}

bool Integer::equals(const Variable *var) const
{
  LOG4CXX_DEBUG(logger, "equals this:Type: " << getType());
  LOG4CXX_DEBUG(logger, "var:Type: " << var->getType());
  assert(getType() == var->getType());

  int ret = (dynamic_cast <const Integer*>(var))->mValue == mValue;

  return ret;
}

void Integer::copy(const Variable *var)
{
  LOG4CXX_DEBUG(logger, "copy this:Type: " << getType());
  LOG4CXX_DEBUG(logger, "var:Type: " << var->getType());
  assert(getType() == var->getType());

  mValue = (dynamic_cast <const Integer*>(var))->mValue;
  setUpdateFlag(true);
}

Variable *Integer::copy() const
{
  return new Integer(mValue);
}

int Integer::getData() const
{
  return mValue;
}

Integer Integer::operator = (const Integer& i)
{
  mValue = i.mValue;
  return *this;
}

//////////////////////////

String::String() :
  Variable(TYPE_STRING)
{
}

String::String(const std::string &s) :
  Variable(TYPE_STRING),
  mValue(s)
{

}

bool String::equals(const Variable *var) const
{
  LOG4CXX_DEBUG(logger, "equals this:Type: " << getType());
  LOG4CXX_DEBUG(logger, "var:Type: " << var->getType());
  assert(getType() == var->getType());

  bool ret = (dynamic_cast <const String*>(var))->mValue == mValue;

  return ret;
}

void String::copy(const Variable *var)
{
  LOG4CXX_DEBUG(logger, "copy this:Type: " << getType());
  LOG4CXX_DEBUG(logger, "var:Type: " << var->getType());
  assert(getType() == var->getType());

  mValue = (dynamic_cast <const String*>(var))->mValue;
  setUpdateFlag(true);
}

Variable *String::copy() const
{
  return new String(mValue);
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

String String::operator = (const String& s)
{
  mValue = s.mValue;
  return *this;
}

//////////////////////////

Struct::Struct() :
  Variable(TYPE_STRUCT)
{
}

Struct::~Struct()
{
  delete_stl_container(mValueMap);
}

bool Struct::equals(const Variable *var) const
{
  LOG4CXX_DEBUG(logger, "equals this:Type: " << getType());
  LOG4CXX_DEBUG(logger, "var:Type: " << var->getType());
  assert(getType() == var->getType());

  // TODO: implement
  bool ret = false;//(dynamic_cast <String*> (var))->mValue == mValue;
  assert(false);

  return ret;
}

void Struct::copy(const Variable *var)
{
  LOG4CXX_DEBUG(logger, "copy this:Type: " << getType());
  LOG4CXX_DEBUG(logger, "var:Type: " << var->getType());
  assert(getType() == var->getType());

  // TODO: implement
  //mValue = (dynamic_cast <String*> (var))->mValue;
  assert(false);
  setUpdateFlag(true);
}

Variable *Struct::copy() const
{
  // TODO: implemenmt
  return NULL;
}

void Struct::add(const std::string &s, Variable *var)
{
  mValueMap[s] = var;
  setUpdateFlag(true);
}

Variable *Struct::getData(const std::string &s)
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
  Variable(TYPE_LIST)
{
}

List::List(const List &list) :
  Variable(TYPE_LIST)
{
  mValueList.resize(list.mValueList.size());
  std::copy(list.mValueList.begin(), list.mValueList.end(), mValueList.begin());
}

List::~List()
{
  
}

bool List::equals(const Variable *var) const
{
  bool ret = false;
  
  if(var)
  {
    LOG4CXX_DEBUG(logger, "equals this:Type: " << getType());
    LOG4CXX_DEBUG(logger, "var:Type: " << var->getType());
    assert(getType() == var->getType());
  
    const List *varList =  dynamic_cast <const List*> (var);

    ret = varList->mValueList == mValueList;
  }
    
  return ret;
}

void List::copy(const Variable *var)
{
  if(var)
  {
    LOG4CXX_DEBUG(logger, "copy this:Type: " << getType());
    LOG4CXX_DEBUG(logger, "var:Type: " << var->getType());
    assert(getType() == var->getType());

    const List *inList = dynamic_cast<const List*>(var);
    
    //delete_stl_container(mValueList);
    std::copy(inList->mValueList.begin(), inList->mValueList.end(), mValueList.begin());
    
    setUpdateFlag(true);
  }
}

Variable *List::copy() const
{
  List *outList = new List(*this);
  
  return outList;
}

void List::pushBack(Variable *var)
{
  mValueList.push_back(var);
  setUpdateFlag(true);
}

void List::pushFront(Variable *var)
{
  mValueList.push_front(var);
  setUpdateFlag(true);
}

void List::clear()
{
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
