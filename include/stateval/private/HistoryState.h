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
  HistoryState (CompoundState *parentState);

  /*!
   * Changes the end of the outgoing history transition.
   * Is called when the state "below" changes.
   */
  void changeTransition (State *state);
  
protected:
  void beforeTransitionCode ();
  void afterTransitionCode ();
  
  void mapEvent (int &inOutEvent);
  
  const Transition *getWalkTransition (int event, bool walkDefaultTransition = true) const;
  
  void runEntryActions ();
  void runExitActions ();
  
private:
};

#endif // HISTORY_STATE_H
