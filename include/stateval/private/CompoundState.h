#ifndef COMPOUND_STATE_H
#define COMPOUND_STATE_H

#include "SimpleState.h"

/* forward declarations */
class HistoryState;

class CompoundState : public SimpleState
{
public:
  CompoundState();
  CompoundState(CompoundState *parentState);

  HistoryState *getHistory();
  void setHistory(HistoryState *hState);

protected:

private:
  HistoryState *mHistoryState;
};

#endif // COMPOUND_STATE_H
