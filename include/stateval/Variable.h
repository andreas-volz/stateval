#ifndef VARIABLE_H
#define VARIABLE_H

/* STD */
#include <iostream>
#include <typeinfo>
#include <map>
#include <list>

class Variable
{
public:
  enum Type
  {
    TYPE_INTEGER,
    TYPE_DOUBLE,
    TYPE_BOOL,
    TYPE_STRING,
    TYPE_LIST,
    TYPE_STRUCT
  };

  virtual ~Variable(){};

  virtual bool equals(const Variable *var) const = 0;
  virtual void copy(const Variable *var) = 0;
  virtual Variable *copy() const = 0;

  Type getType() const;

  bool needsUpdate();
  void setUpdateFlag(bool flag);

protected:
  Variable(Type type);

private:
  Type mType;
  bool mNeedsUpdate;
};

class Bool : public Variable
{
public:
  Bool();
  Bool(bool b);
  
  bool equals(const Variable *var) const;
  void copy(const Variable *var);
  Variable *copy() const;
  
  bool getData() const;

protected:
  bool mValue;
};

//bool operator== (const Bool &b1, const Bool &b2);
//bool operator!= (const Bool &b1, const Bool &b2);

class Double : public Variable
{
public:
  Double();
  Double(double d);

  bool equals(const Variable *var) const;
  void copy(const Variable *var);
  Variable *copy() const;

  double getData() const;

  Double operator = (const Double& d);

protected:
  double mValue;
};

class Integer : public Variable
{
public:
  Integer();
  Integer(int i);

  bool equals(const Variable *var) const;
  void copy(const Variable *var);
  Variable *copy() const;
  
  int getData() const;

  Integer operator = (const Integer& i);

protected:
  int mValue;
};

class String : public Variable
{
public:
  String();
  String(const std::string &s);

  bool equals(const Variable *var) const;
  void copy(const Variable *var);
  Variable *copy() const;

  void change(const std::string &str);

  std::string getData() const;

  String operator = (const String& s);

protected:
  std::string mValue;
};

class Struct : public Variable
{
public:
  typedef std::map <std::string, Variable *>::const_iterator Iterator;

  Struct();
  ~Struct();

  bool equals(const Variable *var) const;
  void copy(const Variable *var);
  Variable *copy() const;
  
  void add(const std::string &s, Variable *var);

  Variable *getData(const std::string &s);
  // TODO: implement [] operator

  //Struct operator = (const Struct& b);

  Iterator begin();
  Iterator end();

protected:
  std::map <std::string, Variable *> mValueMap;
};

// TODO: think about to remove copy() functions and to realize by = operator and std::copy for all types 
class List : public Variable
{
public:
  typedef std::list <Variable *>::const_iterator Iterator;

  List();
  List(const List &list);
  ~List();

  bool equals(const Variable *var) const;
  void copy(const Variable *var);
  Variable *copy() const;

  void pushBack(Variable *var);
  void pushFront(Variable *var);

  void clear();

  Iterator begin();
  Iterator end();

  //List operator = (const List& b);

protected:
  std::list <Variable *> mValueList;
};


#endif // VARIABLE_H
