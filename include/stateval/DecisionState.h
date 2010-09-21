#ifndef DECISION_STATE_H
#define DECISION_STATE_H

/* STD */
#include <list>

/* Project */
#include "State.h"
#include "Condition.h"
#include "Transition.h"

/* forward declaration */

class DecisionState : public State
{
public:
  DecisionState (State *parentState);
  virtual ~DecisionState ();
  
  void addConditionTransition (const std::pair <Condition*, Transition*> &conTrans);

protected:
  void beforeTransitionCode ();
  void afterTransitionCode ();
  
  void mapEvent (int &inOutEvent);

  const Transition *getWalkTransition (int event, bool walkDefaultTransition = true) const;
  
  void runEntryActions ();
  
  void runExitActions ();
  
private:
  std::list <std::pair <Condition*, Transition*> > mConditionList;
  Transition *mElseTrans;
};

#endif // DECISION_STATE_H
