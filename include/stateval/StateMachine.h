#ifndef STATE_MACHINE_H
#define STATE_MACHINE_H

/* STD */
#include <vector>
#include <queue>

/* Project */
#include "State.h"
#include "SMLoader.h"

/* forward declarations */

// the functions for the state machine aren't synchronized. Use StateMachineAccess instead!!!
class StateMachine
{
public:  
  StateMachine ();
  virtual ~StateMachine ();

  void init ();

  bool load (const std::string &smDir);
  
  void pushEvent (int event);
  
  void popEvent ();
  
  void evaluateState (int &inOutEvent);
  
  int findMapingEvent (const std::string &event);
  
  SMLoader& getLoader() {return mSMLoader;}
    
  std::queue <int> eventQueue; // TODO: private
  

private:
  State *searchHierarchie (int event);
  bool walkDown (int event);
  
  State *mActiveState;
  
  SMLoader mSMLoader;
  bool mSMInit;  
};

#endif // STATE_MACHINE_H
