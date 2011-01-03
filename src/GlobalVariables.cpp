#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

/* Project*/
#include "stateval/GlobalVariables.h"
#include "stateval/Variable.h"

/* STD */
#include <cassert>

using namespace std;

// TODO: redesign complete access and locking design!_

GlobalVariables& GlobalVariables::instance()
{
  static GlobalVariables g;
  return g;
}

GlobalVariables::~GlobalVariables ()
{
  // TODO: think about if the Globals should be managed by Loader
  // TODO: clean mVariableList
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
  mutex.lock ();
  mVariableList[str] = &var;
  mutex.unlock ();
}

AbstractVariable *GlobalVariables::getVariable (const std::string &str)
{
  // TODO: return NULL or exception if not found in map
  AbstractVariable *av = mVariableList[str];
  
  return av;
}

void GlobalVariables::changeVariable (const std::string &str, AbstractVariable &av)
{
  mutex.lock ();
  // TODO: throw exception if not found in map
  AbstractVariable *foundVar = mVariableList[str];
  assert (foundVar);

  cout << "change variable: " << str << endl;
  
  foundVar->assign (&av);
  mutex.unlock ();
}
