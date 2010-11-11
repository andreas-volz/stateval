#ifndef VARIABLE_H
#define VARIABLE_H

/* STD */
#include <iostream>
#include <typeinfo>

using namespace std;

class AbstractVariable
{
public:
  enum Type
  {
    TYPE_INTEGER,
    TYPE_FLOAT,
    TYPE_BOOL,
    TYPE_STRING
  };

  virtual bool equals (AbstractVariable *var) const = 0;
  virtual bool assign (AbstractVariable *var) = 0;
  
  Type getType () const;
  
protected:
  AbstractVariable (Type type);
  
private:
  Type mType;
};

class Bool : public AbstractVariable
{
public:
  Bool (bool b);
  
  bool equals (AbstractVariable *var) const;
  bool assign (AbstractVariable *var);
  
private:
  bool mValue;
};

class String : public AbstractVariable
{
public:
  String (const std::string &s);
  
  bool equals (AbstractVariable *var) const;
  bool assign (AbstractVariable *var);
  
private:
  std::string mValue;
};

#endif // VARIABLE_H
