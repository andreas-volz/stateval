#ifndef STATE_MACHINE_LOOP_H
#define STATE_MACHINE_LOOP_H

/* STD */
#include <map>
#include <string>
#include <cassert>

/* local */
#include "stateval/StateMachineAccessor.h"
#include "Thread.h"
#include "Logger.h"

/* forward declarations */
class StateMachine;

class StateMachineLoop
{
public:
  StateMachineLoop(StateMachine &sm);

  /*!
   * The destructor calls cancel() and join()
   */
  ~StateMachineLoop();

  void pushEvent(int event);
  void pushEvent(const std::string &event);

  void start();

  void connect(int event, const SignalSlot &slot);
  void connect(const SignalSlot &slot);
  void disconnect(int event);
  void disconnectAll();

  virtual void run();  // From Thread
  
private:
  
  virtual void signal_cancel(); // from Thread

  void updateEvent(int missedEvents);

  Logger mLogger;

  Threading::Mutex mEventMutex;
  Threading::Condition mEventsInQueue;

  StateMachine *mSM;
  std::multimap <int, SignalSignal *> mSignalList;
  SignalSignal mSignalBroadcast;
};

#endif // STATE_MACHINE_THREAD_H
