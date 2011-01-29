#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

/* local */
#include "stateval/GlobalVariables.h"
#include "stateval/Variable.h"
#include "stateval/private/Logger.h"
#include "MemoryUtil.h"

/* STD */
#include <cassert>

using namespace std;

// TODO: move to header...
static Logger logger ("stateval.GlobalVariables");

// TODO: redesign complete access and locking design!_
// TODO: think about if the Globals should be managed by Loader

GlobalVariables& GlobalVariables::instance()
{
  static GlobalVariables g;
  return g;
}

GlobalVariables::~GlobalVariables ()
{
  delete_stl_container (mVariableList);
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
  // TODO: locking and/or copy value concept
  AbstractVariable *av = mVariableList[str];
  
  return av;
}

void GlobalVariables::changeVariable (const std::string &str, AbstractVariable &av)
{
  mutex.lock ();
  // TODO: throw exception if not found in map
  AbstractVariable *foundVar = mVariableList[str];
  assert (foundVar);

  LOG4CXX_DEBUG (logger, "change variable: " << str);
  
  foundVar->assign (&av);
  mutex.unlock ();
}
