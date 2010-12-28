#ifndef TYPE_H
#define TYPE_H

/* STD */
#include <map>

/* local */
#include "Variable.h"

class Type
{
public:

private:
  std::map <std::string, enum AbstractVariable::Type> mTypeMap;
};

#endif // Type.h
