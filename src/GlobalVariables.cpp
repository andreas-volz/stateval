#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

/* Project*/
#include "stateval/GlobalVariables.h"
#include "stateval/Variable.h"

using namespace std;

GlobalVariables& GlobalVariables::instance()
{
  static GlobalVariables g;
  return g;
}

GlobalVariables::~GlobalVariables ()
{
  // clean mVariableList
  for (map <string,AbstractVariable*>::iterator var_it = mVariableList.begin ();
       var_it != mVariableList.end ();
       ++var_it)
  {
    AbstractVariable *aVar = var_it->second;
    delete aVar;
  }
}

void GlobalVariables::init ()
{
  // put variable initialization here...
}

void GlobalVariables::addVariable (const std::string &str, AbstractVariable &var)
{
  mVariableList[str] = &var;
}

AbstractVariable *GlobalVariables::getVariable (const std::string &str)
{
  // TODO: return NULL or exception is not found in map
  
  return mVariableList[str];
}
