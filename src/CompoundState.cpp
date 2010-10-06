#ifdef HAVE_CONFIG_H
  #include <config.h>
#endif

/* Project */
#include "stateval/CompoundState.h"
#include "stateval/HistoryState.h"

CompoundState::CompoundState () :
  mHistoryState (NULL)
{

}

CompoundState::CompoundState (State *parentState) :
  SimpleState (parentState),
  mHistoryState (NULL)
{

}

HistoryState *CompoundState::getHistory ()
{
  return mHistoryState;
}

void CompoundState::setHistory (HistoryState *hState)
{
  mHistoryState = hState;
}