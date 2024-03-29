#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

/* local */
#include "stateval/private/StateMachine.h"
#include "stateval/private/CompoundState.h"
#include "stateval/private/Exceptions.h"
#include "MemoryUtil.h"
#include "searchFile.h"

/* STD */
#include <iostream>
#include <cassert>

/* pluxx */
#include <pluxx/PluginLoader.h>

using namespace std;

StateMachine::StateMachine(const std::string &loaderPlugin) :
  mLogger("stateval.StateMachine"),
  mActiveState(NULL),  // link to root state
  mSMInit(false)
{
  string pluginFile(searchPluginFile("loaders", loaderPlugin));

  try
  {
    mLoader = (Loader *) pluxx::PluginLoader::loadFactory(pluginFile, "Loader", 1);

    // TODO: correct exception handling!

    LOG4CXX_INFO(mLogger, "Type: " << mLoader->getType());
    LOG4CXX_INFO(mLogger, "Major Version: " << mLoader->getMajorVersion());
    LOG4CXX_INFO(mLogger, "Minor Version: " << mLoader->getMinorVersion());
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

StateMachine::~StateMachine()
{
  // the Loader has to be destroyed in a special way, because it's a plugin!!
  pluxx::PluginLoader::destroyFactory((pluxx::Plugin *) mLoader);
}

void StateMachine::start()
{
  mActiveState = mLoader->getInitialState();
  assert(mActiveState);

  mSMInit = true;
  mLoader->start ();
}

void StateMachine::setDataLoadDir(const std::string &dir)
{
  mLoader->setDataLoadDir(dir);
}

bool StateMachine::load(const std::string &smDir)
{
  return mLoader->load(smDir);
}

void StateMachine::pushEvent(int event)
{
  eventQueue.push(event);
}

void StateMachine::popEvent()
{
  eventQueue.pop();
}

int StateMachine::findMapingEvent(const std::string &event)
{
  return mLoader->findMapingEvent(event);
}

std::string StateMachine::findMapingEvent(int event)
{
  return mLoader->findMapingEvent(event);
}

void StateMachine::evaluateState(int &inOutEvent)
{
  LOG4CXX_DEBUG(mLogger, "Now serving: " << inOutEvent);

  bool transit = walkDown(inOutEvent);

  if (!transit)
  {
    LOG4CXX_DEBUG(mLogger, "nothing found -> searching in hierarchie...");
    assert(mActiveState);

    // map event if state has a view...
    mActiveState->mapEvent(inOutEvent);

    // push event to active state
    // this is e.g. useful to push event to a external GUI event loop
    mActiveState->pushEvent(inOutEvent);

    State *foundState = searchHierarchie(inOutEvent);

    if (foundState != NULL)
    {
      // run exit and entry actions
      mActiveState->runExitActions();

      mActiveState->beforeTransitionCode();

      mActiveState = foundState;
      foundState->runEntryActions();
      // -->

      // do transitions
      walkDown(inOutEvent);
      transit = true;
    }
  }

  if (transit)
  {
    mActiveState->afterTransitionCode();
    LOG4CXX_TRACE(mLogger, "new state: " << mActiveState->getName());
  }
}

State *StateMachine::searchHierarchie(int event)
{
  State *parentState = mActiveState;
  const Transition *trans = NULL;

  do
  {
    // evaluate new active state
    parentState = parentState->getParentState();
    if (parentState == NULL)
      break;

    assert(parentState);

    // doesn't find default transition
    bool walkDefaultTransition = false;
    trans = parentState->getWalkTransition(event, walkDefaultTransition);

    if (trans)
    {
      LOG4CXX_DEBUG(mLogger, "searchHierarchie: father state '" << parentState-> getID() << "' has searched transition");
      LOG4CXX_DEBUG(mLogger, "searchHierarchie: getEvent (): " << trans->getEvent());
      LOG4CXX_DEBUG(mLogger, "searchHierarchie: getEndState (): " << trans->getEndState()->getID());

      return trans->getEndState();  // return found state from hierarchie
    }
    else
    {
      LOG4CXX_DEBUG(mLogger, "searchHierarchie: father state '" << parentState-> getID() << "' hasn't searched event '" << event << "'");
    }
  }
  while (parentState->getParentState() != NULL);

  return NULL; // indicate that search wasn't successful
}

bool StateMachine::walkDown(int event)
{
  const Transition *trans = NULL;
  bool walkDefaultTransition = true;

  // check all possible transitions from current state
  // this loops several times if default transitions are found...
  bool transit = false;
  do
  {
    // evaluate new active state
    assert(mActiveState);

    // map events...
    mActiveState->mapEvent(event);

    walkDefaultTransition = true;
    // this call modifies walkDefaultTransition variable
    trans = mActiveState->getWalkTransition(event, walkDefaultTransition);

    if(!walkDefaultTransition)
    {
      // if default transition was hit consume current event
      event = -1;
    }

    if(trans)
    {
      LOG4CXX_DEBUG(mLogger, "getEvent (): " << trans->getEvent());
      LOG4CXX_DEBUG(mLogger, "getEndState (): " << trans->getEndState());

      LOG4CXX_DEBUG(mLogger, "getName (): " << mActiveState->getName());
      LOG4CXX_DEBUG(mLogger, "getID (): " << mActiveState->getID());

      mActiveState->runExitActions();

      mActiveState->beforeTransitionCode();

      if(mActiveState == trans->getEndState()->getParentState())
      {
        LOG4CXX_WARN(mLogger, "You constructed a loop transition. This may be wrong... " <<
                     "or not if you know what you do (e.g. finish state):" <<
                     mActiveState->getID() << "==" << 
                     trans->getEndState()->getParentState()->getID());
      }

      if (trans->getEndState()->getParentState() == NULL)
      {
        LOG4CXX_INFO(mLogger, "Transition to Root Compound");
        throw StateMachineFinishException();
      }

      mActiveState = trans->getEndState();  // do state change transition

      LOG4CXX_DEBUG(mLogger, "getName (): " << mActiveState->getName());
      LOG4CXX_DEBUG(mLogger, "getID (): " << mActiveState->getID());

      mActiveState->runEntryActions();

      transit = true; // down transition was possible
    }
  }
  while ((trans != NULL));

  return transit;
}

bool StateMachine::hasEvents()
{
  return !eventQueue.empty();
}

unsigned int StateMachine::getEventCount()
{
  return eventQueue.size();
}

int StateMachine::getNextEvent()
{
  return eventQueue.front();
}

void StateMachine::addVariable(const std::string &var, Variable &av)
{
  mLoader->addVariable(var, av);
}

Variable *StateMachine::getVariable(const std::string &var)
{
  return mLoader->getVariable(var);
}

void StateMachine::changeVariable(const std::string &var, Variable &av)
{
  mLoader->changeVariable(var, av);
}

Widget *StateMachine::getWidget(const std::string &view, const std::string &widget)
{
  return mLoader->getWidget(view, widget);
}
