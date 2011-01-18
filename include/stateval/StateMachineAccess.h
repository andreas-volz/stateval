#ifndef STATE_MACHINE_ACCESS_H
#define STATE_MACHINE_ACCESS_H

/* STD */
#include <string>

/* Project */
#include "../include/stateval/StateMachine.h"
#include "../include/stateval/StateMachineThread.h"

/* forward declarations */

/* TODO:
 * Think about how to handle multible running statemachines. New design needed!
 */

// Singleton class
class StateMachineAccess
{
public:
  static StateMachineAccess& instance ();
  
  void init (StateMachine &sm, StateMachineThread &smThread);
  
  void pushEvent (int event);
  void pushEvent (const std::string &event);
  
  int findMapingEvent (const std::string &event);
  std::string findMapingEvent (int event);

  void connect (int event, const SignalSlot& slot);
  void connect (const std::string &event, const SignalSlot& slot);
  void connect (const SignalSlot& slot);
  void disconnect (int event);
  void disconnectAll ();

  bool isInitialized ();
  
private:
  StateMachineAccess () {}
  StateMachineAccess (const StateMachineAccess&);
  virtual ~StateMachineAccess ();
  
  StateMachine *mSM;
  StateMachineThread *mSMThread;
};

#endif // STATE_MACHINE_ACCESS_H
