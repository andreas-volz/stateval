#ifndef STATE_MACHINE_THREAD_H
#define STATE_MACHINE_THREAD_H

/* STD */
#include <map>
#include <string>
#include <cassert>

/* local */
#include "Thread.h"
#include "stateval/StateMachineAccessor.h" 
#include "Logger.h"

/* forward declarations */
class StateMachine;

class StateMachineThread : public Threading::Thread
{
public:
  StateMachineThread (StateMachine &sm);
  
  /*!
   * The destructor calls cancel() and join()
   */
  ~StateMachineThread ();
  
  void pushEvent (int event);
  void pushEvent (const std::string &event);
  
  void start ();  

  void connect (int event, const SignalSlot& slot);
  void connect (const SignalSlot& slot);
  void disconnect (int event);
  void disconnectAll ();
  
private:
  virtual void run (); // From Thread
  virtual void signal_cancel(); // from Thread

  void updateEvent (int missedEvents);

  Logger mLogger;
    
  Threading::Mutex mEventMutex;
  Threading::Condition mEventsInQueue;

  StateMachine *mSM;
  std::multimap <int, SignalSignal*> mSignalList;
  SignalSignal mSignalBroadcast;
};

#endif // STATE_MACHINE_THREAD_H
