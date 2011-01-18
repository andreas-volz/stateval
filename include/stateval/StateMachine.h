#ifndef STATE_MACHINE_H
#define STATE_MACHINE_H

/* STD */
#include <vector>
#include <queue>

/* local */
#include "State.h"
#include "Loader.h"
#include "Context.h"

/* forward declarations */

// the functions for the state machine aren't synchronized. Use StateMachineAccess instead!!!
class StateMachine
{
public:  
  StateMachine (const std::string &loaderPlugin);
  virtual ~StateMachine ();

  void start ();

  bool load (Context *context, const std::string &smDir);
  
  void pushEvent (int event);
  
  void popEvent ();
  
  void evaluateState (int &inOutEvent);
  
  int findMapingEvent (const std::string &event);
  std::string findMapingEvent (int event);

  bool hasEvents ();
  unsigned int getEventCount ();
  int getNextEvent ();

private:
  State *searchHierarchie (int event);
  bool walkDown (int event);

  std::queue <int> eventQueue;
  State *mActiveState;
  Loader *mLoader;
  bool mSMInit;
};

#endif // STATE_MACHINE_H
