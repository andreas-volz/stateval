#ifndef VARIABLE_H
#define VARIABLE_H

/* STD */
#include <iostream>
#include <typeinfo>
#include <map>

using namespace std;

class AbstractVariable
{
public:
  enum Type
  {
    TYPE_INTEGER,
    TYPE_FLOAT,
    TYPE_BOOL,
    TYPE_STRING,
    TYPE_LIST,
    TYPE_STRUCT
  };

  virtual bool equals (AbstractVariable *var) const = 0;
  virtual void assign (AbstractVariable *var) = 0;
  
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
  void assign (AbstractVariable *var);

  bool getData () const;
  
private:
  bool mValue;
};

class Float : public AbstractVariable
{
public:
  Float (float f);
  
  bool equals (AbstractVariable *var) const;
  void assign (AbstractVariable *var);

  float getData () const;
  
private:
  float mValue;
};

class String : public AbstractVariable
{
public:
  String (const std::string &s);
  
  bool equals (AbstractVariable *var) const;
  void assign (AbstractVariable *var);

  std::string getData () const;
  
private:
  std::string mValue;
};

class Struct : public AbstractVariable
{
public:
  Struct (const std::string &s);
  
  bool equals (AbstractVariable *var) const;
  void assign (AbstractVariable *var);
  void add (const std::string &s, AbstractVariable *var);

  AbstractVariable *getData (const std::string &s);
  
private:
  std::map <std::string, AbstractVariable*> mValueMap;
};

#endif // VARIABLE_H
