#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

/* Project */
#include "../include/stateval/HistoryState.h"

HistoryState::HistoryState (State *parentState) :
  State (parentState),
  mHistoryTransition (this) // history points initial to itself
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
  mHistoryTransition.setEndState (state);
}

const Transition *HistoryState::getWalkTransition (int event, bool walkDefaultTransition) const
{
  return &mHistoryTransition;
}

void HistoryState::runEntryActions ()
{
  
}

void HistoryState::runExitActions ()
{
  
}
