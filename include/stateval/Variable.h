#ifndef VARIABLE_H
#define VARIABLE_H

/* STD */
#include <iostream>
#include <typeinfo>
#include <map>
#include <list>

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

  virtual bool equals(AbstractVariable *var) const = 0;
  virtual void assign(AbstractVariable *var) = 0;

  Type getType() const;

  bool needsUpdate ();
  void setUpdateFlag(bool flag);

protected:
  AbstractVariable(Type type);
  
private:
  Type mType;
  bool mNeedsUpdate;
};

class Bool : public AbstractVariable
{
public:
  Bool(bool b);

  bool equals(AbstractVariable *var) const;
  void assign(AbstractVariable *var);

  bool getData() const;

private:
  bool mValue;
};

// TODO: rename to Double...
class Float : public AbstractVariable
{
public:
  Float(float f);

  bool equals(AbstractVariable *var) const;
  void assign(AbstractVariable *var);

  float getData() const;

private:
  float mValue;
};

class Integer : public AbstractVariable
{
public:
  Integer(int i);

  bool equals(AbstractVariable *var) const;
  void assign(AbstractVariable *var);

  int getData() const;

private:
  int mValue;
};

class String : public AbstractVariable
{
public:
  String(const std::string &s);

  bool equals(AbstractVariable *var) const;
  void assign(AbstractVariable *var);

  void change(const std::string &str);

  std::string getData() const;

private:
  std::string mValue;
};

class Struct : public AbstractVariable
{
public:
  typedef std::map <std::string, AbstractVariable *>::const_iterator Iterator;

  Struct();
  ~Struct();

  bool equals(AbstractVariable *var) const;
  void assign(AbstractVariable *var);
  void add(const std::string &s, AbstractVariable *var);

  AbstractVariable *getData(const std::string &s);
  // TODO: implement [] operator

  Iterator begin();
  Iterator end();

private:
  std::map <std::string, AbstractVariable *> mValueMap;
};

class List : public AbstractVariable
{
public:
  typedef std::list <AbstractVariable *>::const_iterator Iterator;

  List();
  ~List();

  bool equals(AbstractVariable *var) const;
  void assign(AbstractVariable *var);

  void pushBack(AbstractVariable *var);
  void pushFront(AbstractVariable *var);

  void clear();

  Iterator begin();
  Iterator end();

private:
  std::list <AbstractVariable *> mValueList;
};


#endif // VARIABLE_H
