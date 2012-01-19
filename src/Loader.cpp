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
#include "MemoryUtil.h"

#include "stringUtil.h"
#include "ViewPluginLoader.h"

using namespace std;

Loader::Loader() :
  mLogger("stateval.Loader"),
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

  // the view has to be destroyed in a special way because it's a plugin!!
  for (std::vector <View *>::iterator view_it = mViewList.begin();
       view_it != mViewList.end();
       ++view_it)
  {
    View *view = *view_it;
    pluxx::PluginLoader::destroyFactory(view);
  }
  mViewList.clear();
  // <-
}

void Loader::addEvent(const std::string &event)
{
  std::map <std::string, int>::iterator ev_it = mEventList.find(event);

  if (ev_it == mEventList.end())  // not found
  {
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

void Loader::addView(View *view)
{
  mViewList.push_back(view);
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
    //cout << "map event: " << iter->first << " : " << iter->second << endl;
    return mapEvent;
  }

  LOG4CXX_DEBUG(mLogger, "StateMachine::findMapingEvent: try to find not existing event: " << event);

  return -1;
}

std::string Loader::findMapingEvent(int event)
{
  if (static_cast <int> (mEventListIndex.size()) >= event)
  {
    return mEventListIndex[event];
  }

  return ""; // TODO: hm, should I return an Exception or so??
}

View *Loader::loadView(const std::string &viewPlugin, Context *context, const std::map <std::string, std::string> &params)
{
  View *view = NULL;

  try
  {
    view = (View *) ViewPluginLoader::loadFactory(viewPlugin, "View", 1,
           context, params);

    // TODO: correct exception handling!

    LOG4CXX_INFO(mLogger, "Type: " <<  view->getType());
    LOG4CXX_INFO(mLogger, "Major Version: " << view->getMajorVersion());
    LOG4CXX_INFO(mLogger, "Minor Version: " << view->getMinorVersion());
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

  return view;
}
