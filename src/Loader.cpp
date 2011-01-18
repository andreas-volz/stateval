#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

/* STD */
#include <iostream>
#include <cassert>

/* pluxx */
#include <pluxx/PluginLoader.h>

/* local */
#include "stateval/Loader.h"
#include "stateval/CompoundState.h"
#include "stateval/HistoryState.h"
#include "stateval/DecisionState.h"
#include "stateval/ViewState.h"
#include "Logger.h"
#include "MemoryUtil.h"

#include "stringUtil.h"
#include "ViewPluginLoader.h"

using namespace std;

static Logger logger ("stateval.Loader");

Loader::Loader () :
  eventCounter (0)
{
}

Loader::~Loader ()
{
  LOG4CXX_TRACE (logger, "~Loader");

  // -> Free all statemachine data containers at destruction time
  delete_stl_container (mStateList);
  delete_stl_container (mViewList);
  delete_stl_container (mActionList);
  // <-
}

void Loader::addEvent (const std::string &event)
{
  mEventList[event] = eventCounter;
  mEventListIndex.push_back (event);
    
  ++eventCounter;
}

void Loader::addState (State *state)
{
  mStateList.push_back (state);
}

void Loader::addAction (Action *action)
{
  mActionList.push_back (action);
}

void Loader::addView (View *view)
{
  mViewList.push_back (view);
}

State *Loader::getInitialState ()
{
  return *mStateList.begin ();
}

int Loader::findMapingEvent (const std::string &event)
{
  map <string,int>::iterator iter = mEventList.find(event);
  if (iter != mEventList.end())
  {
    int &mapEvent = iter->second;
    //cout << "map event: " << iter->first << " : " << iter->second << endl;
    return mapEvent;
  }

  LOG4CXX_DEBUG (logger, "StateMachine::findMapingEvent: try to find not existing event: " << event);

  return -1;
}

std::string Loader::findMapingEvent (int event)
{
  if (mEventListIndex.size () >= event)
  {    
    return mEventListIndex[event];
  }
  
  return ""; // TODO: hm, should I return an Exception or so??
}

View *Loader::loadView (const std::string &viewPlugin, Context *context, const std::list <std::string> &params)
{  
  View *view = NULL;
  
  try
  {
    view = (View*) ViewPluginLoader::loadFactory (viewPlugin, "View", 1,
                                                  context, params);

    // TODO: correct exception handling!
    
    LOG4CXX_INFO (logger, "Type: " <<  view->getType ());
    LOG4CXX_INFO (logger, "Major Version: " << view->getMajorVersion ());
    LOG4CXX_INFO (logger, "Minor Version: " << view->getMinorVersion ());
  }
  catch (pluxx::PluginTypeMismatchException typeEx)
  {
    LOG4CXX_FATAL (logger, "catched an PluginTypeMismatchException exception...");
    LOG4CXX_FATAL (logger, "Loader Type: " << typeEx.getLoaderType ());
    LOG4CXX_FATAL (logger, "Plugin Type: " << typeEx.getPluginType ());
  }
  catch (pluxx::PluginMajorVersionMismatchException verEx)
  {
    LOG4CXX_FATAL (logger, "catched an PluginMajorVersionMismatchException exception...");
    LOG4CXX_FATAL (logger, "Loader Major Version: " << verEx.getLoaderMajorVersion ());
    LOG4CXX_FATAL (logger, "Plugin Major Version: " << verEx.getPluginMajorVersion ());
  }

  return view;
}
