#ifndef STATE_MACHINE_ACCESSOR_H
#define STATE_MACHINE_ACCESSOR_H

/* STD */
#include <string>

/* Project */
#include "stateval/private/StateMachine.h"
#include "stateval/private/StateMachineThread.h"

/* forward declarations */

/* TODO:
 * Think about how to handle multible running statemachines. New design needed!
 */

// Singleton class
class StateMachineAccessor
{
public:
  static StateMachineAccessor& getInstance ();
  
  void load (const std::string &loader, const std::string &file, Context *context);

  void start ();
  
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
  StateMachineAccessor () {}
  StateMachineAccessor (const StateMachineAccessor&);
  virtual ~StateMachineAccessor () {};
  
  StateMachine *mSM;
  StateMachineThread *mSMThread;
};

#endif // STATE_MACHINE_ACCESSOR_H
