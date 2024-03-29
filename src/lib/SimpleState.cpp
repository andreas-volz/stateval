#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

/* Project */
#include "stateval/private/SimpleState.h"
#include "stateval/private/CompoundState.h"
#include "stateval/private/HistoryState.h"
#include "MemoryUtil.h"

/* STD */
#include <iostream>
#include <cassert>

using namespace std;

SimpleState::SimpleState() :
  mLogger("stateval.SimpleState")
{
}

SimpleState::SimpleState(CompoundState *parentState) :
  State(parentState),
  mLogger("stateval.SimpleState")
{
}

SimpleState::~SimpleState()
{
}

void SimpleState::addEntryAction(Action *action)
{
  if (action)
  {
    mEntryActionList.push_back(action);
  }
}

void SimpleState::addExitAction(Action *action)
{
  if (action)
  {
    mExitActionList.push_back(action);
  }
}

void SimpleState::runEntryActions()
{
  LOG4CXX_TRACE(mLogger, "runEntryActions()");
  runActions(mEntryActionList);
}

void SimpleState::runExitActions()
{
  runActions(mExitActionList);
}

void SimpleState::runActions(std::list <Action *> &actionList)
{
  for (std::list <Action *>::const_iterator ac_it = actionList.begin();
       ac_it != actionList.end();
       ++ac_it)
  {
    const Action *action = *ac_it;
    assert(action);
    LOG4CXX_TRACE(mLogger, "Run Action");
    action->run();
  }
}

const Transition *SimpleState::getWalkTransition(int event, bool &walkDefaultTransition) const
{
  const Transition *defaultTransition = NULL;

  for (std::list <Transition *>::const_iterator tr_it = mLeaveTransitonList.begin();
       tr_it != mLeaveTransitonList.end();
       ++tr_it)
  {
    const Transition *trans = *tr_it;
    int tevent = trans->getEvent();

    if (tevent == event)
    {
      walkDefaultTransition = false;
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

void SimpleState::beforeTransitionCode()
{
}

void SimpleState::afterTransitionCode()
{
  changeHistory();
}

void SimpleState::mapEvent(int &inOutEvent)
{
}

void SimpleState::changeHistory()
{
  if (mParentState)
  {
    HistoryState *hState = mParentState->getHistory();

    if (hState)
    {
      LOG4CXX_TRACE(mLogger, "change history transition");
      hState->changeTransition(this);
    }
  }
}
