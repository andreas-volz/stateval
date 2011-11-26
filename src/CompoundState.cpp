#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

/* Project */
#include "stateval/private/CompoundState.h"
#include "stateval/private/HistoryState.h"

CompoundState::CompoundState() :
  mHistoryState(NULL)
{
}

CompoundState::CompoundState(CompoundState *parentState) :
  SimpleState(parentState),
  mHistoryState(NULL)
{
}

HistoryState *CompoundState::getHistory()
{
  return mHistoryState;
}

void CompoundState::setHistory(HistoryState *hState)
{
  mHistoryState = hState;
}
