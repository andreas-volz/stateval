#ifndef SIMPLE_STATE_H
#define SIMPLE_STATE_H

/* STD */
#include <list>

/* Project */
#include "Transition.h"
#include "Action.h"
#include "State.h"

class SimpleState : public State
{
public:
  SimpleState (State *parentState);
  virtual ~SimpleState ();

  void beforeTransitionCode ();
  
  void addEntryAction (Action *action);
  void addExitAction (Action *action);

protected:
  SimpleState ();

  void changeHistory ();
  
private:
  void afterTransitionCode ();
  
  void mapEvent (int &inOutEvent);

  const Transition *getWalkTransition (int event, bool walkDefaultTransition = true) const;
  
  void runEntryActions ();
  void runExitActions ();
  
  void runActions (std::list <Action*> &actionList);
  
  std::list <Action*> mEntryActionList; // pointers are freed by Loader!
  std::list <Action*> mExitActionList; // pointers are freed by Loader!
};

#endif // SIMPLE_STATE_H