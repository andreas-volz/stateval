#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

/* STD */
#include <iostream>
#include <cassert>

/* pluxx */
#include <pluxx/PluginLoader.h>

/* local */
#include "stateval/private/Loader.h"
#include "stateval/private/CompoundState.h"
#include "stateval/private/HistoryState.h"
#include "stateval/private/DecisionState.h"
#include "stateval/private/ViewState.h"
#include "stateval/StateMachineAccessor.h"
#include "MemoryUtil.h"
#include "stringUtil.h"
#include "ViewManagerPluginLoader.h"
#include "searchFile.h"

using namespace std;

Loader::Loader() :
  mLogger("stateval.Loader"),
  mViewManager (NULL),
  eventCounter(0)
{
}

Loader::~Loader()
{
  LOG4CXX_TRACE(mLogger, "~Loader");

  // -> Free all statemachine data containers at destruction time
  delete_stl_container(mVariableList);
  delete_stl_container(mActionList);
  delete_stl_container(mStateList);

  // the ViewManager has to be destroyed in a special way because it's a plugin!!
  pluxx::PluginLoader::destroyFactory(mViewManager);  
}

void Loader::addEvent(const std::string &event)
{
  std::map <std::string, int>::iterator ev_it = mEventList.find(event);

  if (ev_it == mEventList.end())  // not found
  {
    LOG4CXX_DEBUG(mLogger, "Add event '" << event << "' with index '" << eventCounter << "'");
    mEventList[event] = eventCounter;
    mEventListIndex.push_back(event);
    ++eventCounter;
  }
  else
  {
    LOG4CXX_WARN(mLogger, "Try to add same event more than once in map: " << event);
  }
}

void Loader::addState(State *state)
{
  mStateList.push_back(state);
}

void Loader::addAction(Action *action)
{
  mActionList.push_back(action);
}

void Loader::addVariable(const std::string &var, AbstractVariable &av)
{
  std::map <std::string, AbstractVariable *>::iterator var_it = mVariableList.find(var);

  if (var_it == mVariableList.end())  // not found
  {
    mVariableList[var] = &av;
  }
  else
  {
    LOG4CXX_WARN(mLogger, "Try to add same variable more than once in map: " << var);
  }
}

AbstractVariable *Loader::getVariable(const std::string &var)
{
  std::map <std::string, AbstractVariable *>::iterator var_it = mVariableList.find(var);

  if (var_it == mVariableList.end())  // not found
  {
    // TODO: do something smarter later, but for now assert...
    assert(false);
  }

  // else...

  // TODO: locking and/or copy value concept
  AbstractVariable *av = mVariableList[var];

  return av;
}

void Loader::changeVariable(const std::string &var, AbstractVariable &av)
{
  //mutex.lock ();
  // TODO: throw exception if not found in map
  AbstractVariable *foundVar = mVariableList[var];
  assert(foundVar);

  LOG4CXX_DEBUG(mLogger, "change variable: " << var);

  foundVar->assign(&av);
  //mutex.unlock ();
}

State *Loader::getInitialState()
{
  return *mStateList.begin();
}

int Loader::findMapingEvent(const std::string &event)
{
  map <string, int>::iterator iter = mEventList.find(event);
  if (iter != mEventList.end())
  {
    int &mapEvent = iter->second;
    return mapEvent;
  }

  LOG4CXX_DEBUG(mLogger, "StateMachine::findMapingEvent: try to find not existing event: " << event);

  // don't use exceptions here because this case happens really often and is not an error!
  return -1;
}

std::string Loader::findMapingEvent(int event)
{
  if (static_cast <int> (mEventListIndex.size()) >= event)
  {
    return mEventListIndex[event];
  }

  assert (false); // fix this next time I hit this assert...
  return ""; // TODO: hm, should I return an Exception or so??
}

void Loader::loadViewManager(const std::string &viewmanagerPlugin, const std::map <std::string, std::string> &params)
{
  try
  {
    string pluginFile(searchPluginFile("viewmanager", viewmanagerPlugin));
    mViewManager = (ViewManager *) ViewManagerPluginLoader::loadFactory(pluginFile, "ViewManager", 1, params);

    // TODO: correct exception handling!

    LOG4CXX_INFO(mLogger, "Type: " <<  mViewManager->getType());
    LOG4CXX_INFO(mLogger, "Major Version: " << mViewManager->getMajorVersion());
    LOG4CXX_INFO(mLogger, "Minor Version: " << mViewManager->getMinorVersion());
  }
  catch (pluxx::PluginTypeMismatchException typeEx)
  {
    LOG4CXX_FATAL(mLogger, "catched an PluginTypeMismatchException exception...");
    LOG4CXX_FATAL(mLogger, "Loader Type: " << typeEx.getLoaderType());
    LOG4CXX_FATAL(mLogger, "Plugin Type: " << typeEx.getPluginType());
  }
  catch (pluxx::PluginMajorVersionMismatchException verEx)
  {
    LOG4CXX_FATAL(mLogger, "catched an PluginMajorVersionMismatchException exception...");
    LOG4CXX_FATAL(mLogger, "Loader Major Version: " << verEx.getLoaderMajorVersion());
    LOG4CXX_FATAL(mLogger, "Plugin Major Version: " << verEx.getPluginMajorVersion());
  }
}

void Loader::start ()
{
  // inital event ->
  StateMachineAccessor &StateMachineAccessor(StateMachineAccessor::getInstance());  
  cout << "initial event" << endl;
  StateMachineAccessor.pushEvent(-1);
  // <-
}
