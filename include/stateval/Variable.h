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

  virtual bool compare (AbstractVariable *var) const = 0;
  
  Type getType () const;
  
protected:
  AbstractVariable (Type type);
  
private:
  Type mType;
};

/*template <typename T>
class Variable : public AbstractVariable
{
public:
  Variable (const T &val);

  void setValue (const T &v);
  T &getValue ();
  
  bool compare (AbstractVariable *var) const;
  
private:
  T mVal;
};

template <typename T>
Variable<T>::Variable (const T &val)
{
  mVal = val;
}

template <typename T>
void Variable<T>::setValue (const T &v)
{
  mVal = v;
}

template <typename T>
T &Variable<T>::getValue ()
{
  return mVal;
}

template <typename T>
bool Variable<T>::compare (AbstractVariable *var) const
{
  //T *t = static_cast <T*> (var);
  
  cout << "this: " << typeid(this).name() << endl;
  cout << "var: " << typeid(var).name() << endl;
  bool b = typeid(this) == typeid(var);
  cout << "== " << b  << endl;
}

typedef Variable <int> Integer;
typedef Variable <float> Float;
typedef Variable <bool> Bool;*/

class Bool : public AbstractVariable
{
public:
  Bool (bool b);
  
  bool compare (AbstractVariable *var) const;
  
private:
  bool mValue;
};



#endif // VARIABLE_H
