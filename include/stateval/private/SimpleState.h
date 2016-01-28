#ifndef SIMPLE_STATE_H
#define SIMPLE_STATE_H

/* STD */
#include <list>

/* Project */
#include "Transition.h"
#include "Action.h"
#include "State.h"
#include "stateval/private/Logger.h"

class SimpleState : public State
{
public:
  SimpleState(CompoundState *parentState);
  virtual ~SimpleState();

  void beforeTransitionCode();

  void addEntryAction(Action *action);
  void addExitAction(Action *action);

protected:
  SimpleState();

  void changeHistory();

private:
  void afterTransitionCode();

  void mapEvent(int &inOutEvent);

  const Transition *getWalkTransition(int event, bool &walkDefaultTransition) const;

  void runEntryActions();
  void runExitActions();

  void runActions(std::list <Action *> &actionList);

  Logger mLogger;
  
  std::list <Action *> mEntryActionList; // pointers are freed by Loader!
  std::list <Action *> mExitActionList; // pointers are freed by Loader!
};

#endif // SIMPLE_STATE_H