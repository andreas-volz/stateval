#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

/* Project */
#include "../include/stateval/SimpleState.h"
#include "../include/stateval/CompoundState.h"
#include "../include/stateval/HistoryState.h"
#include "localUtil.h"

/* STD */
#include <iostream>
#include <cassert>

using namespace std;

SimpleState::SimpleState ()
{
}

SimpleState::SimpleState (State *parentState) :
  State (parentState)
{
}

SimpleState::~SimpleState ()
{
  //cout << "clean up SimpleState" << endl;
  
  // free mEntryActionList
  delete_stl_container <std::list <Action*>, Action*> (mEntryActionList);
  
  // free mExitActionList
  delete_stl_container <std::list <Action*>, Action*> (mExitActionList);
}

void SimpleState::addEntryAction (Action *action)
{
  if (action)
  {
    mEntryActionList.push_back (action);
  }
}

void SimpleState::addExitAction (Action *action)
{
  if (action)
  {
    mExitActionList.push_back (action);
  }
}

void SimpleState::runEntryActions ()
{
  runActions (mEntryActionList);
}

void SimpleState::runExitActions ()
{
  runActions (mExitActionList);
}

void SimpleState::runActions (std::list <Action*> &actionList)
{
  for (std::list <Action*>::const_iterator ac_it = actionList.begin ();
     ac_it != actionList.end ();
     ++ac_it)
  {
    const Action *action = *ac_it;
    assert (action);
    action->run ();
  }
}

const Transition *SimpleState::getWalkTransition (int event, bool walkDefaultTransition) const
{
  const Transition *defaultTransition = NULL;
  
  for (std::list <Transition*>::const_iterator tr_it = mLeaveTransitonList.begin ();
       tr_it != mLeaveTransitonList.end ();
       ++tr_it)
  {
    const Transition *trans = *tr_it;
    int tevent = trans->getEvent ();
    
    if (tevent == event)
    {      
      return trans; 
    }
    else if (tevent == -1) // default transition
    {
      defaultTransition = trans;
    }
  }
  
  // if no Transition for a event was found then return the default Transition
  // if one is available...
  if (defaultTransition && walkDefaultTransition)
  {
    return defaultTransition;
  }
  
  return NULL; // really no walk transition for this state found
}

void SimpleState::beforeTransitionCode ()
{ 
}

void SimpleState::afterTransitionCode ()
{
  changeHistory ();
}

void SimpleState::mapEvent (int &inOutEvent)
{
}

void SimpleState::changeHistory ()
{
  if (mParentState)
  {
    CompoundState *cState = static_cast <CompoundState*> (mParentState); // TODO: cast not needed if mParentState would be CompoundState
    HistoryState *hState = cState->getHistory ();
      
    if (hState)
    {
      cout << "change history transition" << endl;
      hState->changeTransition (this);
    }
  }
}
