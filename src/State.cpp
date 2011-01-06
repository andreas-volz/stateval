#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

/* Project */
#include "stateval/State.h"
#include "MemoryUtil.h"

/* STD */
#include <iostream>

using namespace std;

State::State () :
  mParentState (NULL), // => ROOT
  mID (0)
{ 
}

State::State (State *parentState) :
  mParentState (parentState),
  mID (0)
{
}

State::~State ()
{
  // TODO: think about if the Transitions should be managed by Loader
  
  delete_stl_container (mLeaveTransitonList);
}

void State::setID (unsigned int id)
{
  mID = id;
}
  
unsigned int State::getID ()
{
  return mID;
}

void State::setName (const std::string name)
{
  mName = name;
}

const std::string State::getName ()
{
  return mName;
}

void State::addEntryAction (Action *action)
{
  // this is implemented empty with the intention that child classes
  // don't support actions by default
  // e.g. SimpleState overwrites it to support actions
}

void State::addExitAction (Action *action)
{
  // this is implemented empty with the intention that child classes
  // don't support actions by default
  // e.g. SimpleState overwrites it to support actions
}

void State::updateContent ()
{
  // this is implemented empty with the intention that child classes
  // don't support any content by default
  // e.g. some ViewState types overwrites it to support content update
}

State *State::getParentState () const
{
  return mParentState;
}

void State::addLeaveTransition (Transition &trans)
{
  mLeaveTransitonList.push_back (&trans);
}
