#ifndef STATE_H
#define STATE_H

/* STD */
#include <list>

/* Local */
#include "Transition.h"
#include "Action.h"
#include "View.h"

/* forward declarations */
class CompoundState;

class State
{
public:
  virtual ~State ();
  
  friend class StateMachine;
    
  virtual void addLeaveTransition (Transition &trans);
  
  // setID and getID has no functional need. It's only useful to debug the statemachine
  // all works nice without setting this!
  void setID (unsigned int id);
  unsigned int getID ();

  // setName and getName has no functional need. It's only useful to debug the statemachine
  // all works nice without setting this!
  void setName (const std::string name);
  const std::string getName ();

  virtual void addEntryAction (Action *action);
  virtual void addExitAction (Action *action);
  
protected:
  virtual CompoundState *getParentState () const;
  
  virtual void beforeTransitionCode () = 0;
  
  virtual void afterTransitionCode () = 0;
  
  virtual void mapEvent (int &inOutEvent) = 0;
  
  virtual const Transition *getWalkTransition (int event, bool walkDefaultTransition = true) const = 0;
  
  virtual void runEntryActions () = 0;
  virtual void runExitActions () = 0;

  virtual void pushEvent (int event);
  
protected:
  // TODO: why are the constructors private?
  State ();
  State (CompoundState *parentState);
  
  CompoundState *mParentState; // NULL == root // TODO: why not CompoundState possible???
  std::list <Transition*> mLeaveTransitonList;
  unsigned int mID; // Debug
  std::string mName; // Debug
};

#endif // STATE_H
