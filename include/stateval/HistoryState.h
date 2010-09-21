#ifndef HISTORY_STATE_H
#define HISTORY_STATE_H

/* Project */
#include "State.h"

/* forward declarations */
class SimpleState;
class CompoundState;
class ViewState;

class HistoryState : public State
{
public:
  HistoryState (State *parentState);

  void changeTransition (State *state); // TODO: other design or friend!
  
protected:
  void beforeTransitionCode ();
  void afterTransitionCode ();
  
  void mapEvent (int &inOutEvent);
  
  const Transition *getWalkTransition (int event, bool walkDefaultTransition = true) const;
  
  void runEntryActions ();
  void runExitActions ();
  
private:
  Transition mHistoryTransition;
};

#endif // HISTORY_STATE_H
