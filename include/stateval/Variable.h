#ifndef VARIABLE_H
#define VARIABLE_H

/* STD */
#include <iostream>
#include <typeinfo>
#include <map>
#include <list>

// TODO: think about a variable_cast<> function like static_cast<> with type check

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
    TYPE_STRUCT,
    TYPE_VOIDPTR
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

  friend std::ostream& operator<< (std::ostream &out, Bool &b);
  friend std::ostream& operator<< (std::ostream &out, Variable &v);

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

  friend std::ostream& operator<< (std::ostream &out, Double &d);

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

  friend std::ostream& operator<< (std::ostream &out, Integer &i);

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

  friend std::ostream& operator<< (std::ostream &out, String &s);

protected:
  std::string mValue;
};

class VoidPtr : public Variable
{
public:
  VoidPtr();
  VoidPtr(void *v);

  bool equals(const Variable *var) const;
  void copy(const Variable *var);
  Variable *copy() const;

  void change(void* v);

  bool isZero() const;

  void *getData() const;

  VoidPtr operator = (const VoidPtr& v);

protected:
  void *mValue;
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

  Variable *get(const std::string &s);
  // TODO: implement [] operator

  //Struct operator = (const Struct& b);

  Iterator begin();
  Iterator end();

  //friend std::ostream& operator<< (std::ostream &out, Struct &s);

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
  // TODO: implement [] operator

  friend std::ostream& operator<< (std::ostream &out, List &l);

protected:
  std::list <Variable *> mValueList;
};


#endif // VARIABLE_H
