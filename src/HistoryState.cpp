#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

/* Project */
#include "stateval/HistoryState.h"

/* STD */
#include <cassert>

HistoryState::HistoryState (CompoundState *parentState) :
  State (parentState)
{
  
}

void HistoryState::beforeTransitionCode ()
{
}

void HistoryState::afterTransitionCode ()
{
}

void HistoryState::mapEvent (int &inOutEvent)
{
  
}

void HistoryState::changeTransition (State *state)
{
  // warn while development time...
  assert (mLeaveTransitonList.size () == 1);
  
  // ignore more than one and take first one
  if (mLeaveTransitonList.size () >= 1)
  {
    Transition *historyTrans = *(mLeaveTransitonList.begin ());

    // only react on default transition
    if (historyTrans->getEvent () == Transition::NoEvent)
    {
      historyTrans->setEndState (state);
    }
  }
}

const Transition *HistoryState::getWalkTransition (int event, bool walkDefaultTransition) const
{
  Transition *historyTrans = NULL;

  // warn while development time...
  assert (mLeaveTransitonList.size () == 1);
  
  // ignore more than one and take first one
  if (mLeaveTransitonList.size () >= 1)
  {
    historyTrans = *(mLeaveTransitonList.begin ());

    // only react on default transition
    if (historyTrans->getEvent () != Transition::NoEvent)
    {
      // a history without outgoing transition isn't really useful...
      historyTrans = NULL;
    }
  }

  return historyTrans;
}

void HistoryState::runEntryActions ()
{
  
}

void HistoryState::runExitActions ()
{
  
}
