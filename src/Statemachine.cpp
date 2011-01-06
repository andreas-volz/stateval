#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

/* local */
#include "stateval/StateMachine.h"
#include "MemoryUtil.h"
#include "searchFile.h"

/* STD */
#include <iostream>
#include <cassert>

/* pluxx */
#include <pluxx/PluginLoader.h>

using namespace std;

StateMachine::StateMachine (const std::string &loaderPlugin) :
  mActiveState (NULL), // link to root state
  mSMInit (false)
{
  string pluginFile (searchPluginFile ("loaders", loaderPlugin));
  
  try
  {
    mLoader = (Loader*) pluxx::PluginLoader::loadFactory (pluginFile, "Loader", 1);

    // TODO: correct exception handling!
    
    cout << "Type: " << mLoader->getType () << endl;
    cout << "Major Version: " << mLoader->getMajorVersion () << endl;
    cout << "Minor Version: " << mLoader->getMinorVersion () << endl;
  }
  catch (pluxx::PluginTypeMismatchException typeEx)
  {
    cout << "catched an PluginTypeMismatchException exception..." << endl;
    cout << "Loader Type: " << typeEx.getLoaderType () << endl;
    cout << "Plugin Type: " << typeEx.getPluginType () << endl;
  }
  catch (pluxx::PluginMajorVersionMismatchException verEx)
  {
    cout << "catched an PluginMajorVersionMismatchException exception..." << endl;
    cout << "Loader Major Version: " << verEx.getLoaderMajorVersion () << endl;
    cout << "Plugin Major Version: " << verEx.getPluginMajorVersion () << endl;
  }
}

StateMachine::~StateMachine ()
{
  pluxx::PluginLoader::destroyFactory ((pluxx::Plugin*) mLoader);  
}

void StateMachine::init ()
{
  mActiveState = mLoader->getInitialState ();
  assert (mActiveState);
  
  mSMInit = true;
}

bool StateMachine::load (Context *context, const std::string &smDir)
{
  return mLoader->load (context, smDir);
}

void StateMachine::pushEvent (int event)
{
  eventQueue.push (event);
}

void StateMachine::popEvent ()
{
  eventQueue.pop();
}

int StateMachine::findMapingEvent (const std::string &event)
{
  return mLoader->findMapingEvent (event);
}

void StateMachine::evaluateState (int &inOutEvent)
{
  const Transition *trans = NULL;
  
  cout << "Now serving: " << inOutEvent << endl;
  //cout << "activeState (before): " << mActiveState << endl;
  
  bool transit = walkDown (inOutEvent);

  /// FIXME!!!!!! not every event!!!
  //if (mActiveState)
    //mActiveState->updateContent ();
  
  if (!transit)
  {
    cout << "nothing found -> searching in hierarchie..." << endl;
    assert (mActiveState);
    
    // map event if state has a view...
    mActiveState->mapEvent (inOutEvent);
    
    State *foundState = searchHierarchie (inOutEvent);
    //cout << "found state that fits: " << foundState << endl;
    
    if (foundState != NULL)
    {
      // run exit and entry actions
      mActiveState->runExitActions ();
      
      mActiveState->beforeTransitionCode ();
      
      mActiveState = foundState;
      foundState->runEntryActions ();
      // -->
      
      // do transitions
      walkDown (inOutEvent);
      transit = true;
    }
  }

  if (transit)
  {
    mActiveState->afterTransitionCode ();
  }
}

State *StateMachine::searchHierarchie (int event)
{
  State *parentState = mActiveState;
  const Transition *trans = NULL;
  
  do
  {
    // evaluate new active state
    parentState = parentState->getParentState ();
    if (parentState == NULL)
        break;
    assert (parentState);
    
    // doesn't find default transition
    trans = parentState->getWalkTransition (event, false);
    
    if (trans)
    {
      cout << "StateMachine::searchHierarchie: father state '" << parentState-> getID () << "' has searched transition" << endl;
      cout << "StateMachine::searchHierarchie: getEvent (): " << trans->getEvent () << endl;
      cout << "StateMachine::searchHierarchie: getEndState (): " << trans->getEndState ()->getID () << endl;
      
      return trans->getEndState (); // return found state from hierarchie
    }
    else
    {
      cout << "StateMachine::searchHierarchie: father state '" << parentState-> getID () << "' hasn't searched event '" << event << "'" << endl;
    }
  }
  while (parentState->getParentState () != NULL);

  return NULL; // indicate that search wasn't successful
}

bool StateMachine::walkDown (int event)
{
  // check all possible transitions from current state 
  // this loops several times if default transitions are found... ->
  const Transition *trans = NULL;

  bool transit = false;
  do
  {
    // evaluate new active state
    cout << "evaluate new active state" << endl;
    assert (mActiveState);

    // map events...
    mActiveState->mapEvent (event);
    
    trans = mActiveState->getWalkTransition (event);
    
    if (trans)
    {
      cout << "getEvent (): " << trans->getEvent () << endl;
      cout << "getEndState (): " << trans->getEndState () << endl;
      
      cout << "getName (): " << mActiveState->getName () << endl;
      cout << "getID (): " << mActiveState->getID () << endl;

      mActiveState->runExitActions ();
      
      mActiveState->beforeTransitionCode ();
      
      mActiveState = trans->getEndState (); // do state change transition

      cout << "getName (): " << mActiveState->getName () << endl;
      cout << "getID (): " << mActiveState->getID () << endl;
      
      mActiveState->runEntryActions ();
      
      transit = true; // down transition was possible
    }
  }
  while (trans != NULL);
  // <-
  
  return transit;
}
