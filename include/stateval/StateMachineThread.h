#ifndef STATE_MACHINE_THREAD_H
#define STATE_MACHINE_THREAD_H

/* STD */
#include <map>

/* SIGC */
#include <sigc++/sigc++.h>

/* GLIB */
#include <glibmm/thread.h>

/* Project */
#include "EcoreDispatcher.h"

typedef sigc::signal<void, int> SignalSignal;
typedef sigc::slot1<void, int> SignalSlot;

/* forward declarations */
class StateMachine;

class StateMachineThread
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
  void cancel ();
  void join ();

	void connect (int event, const SignalSlot& slot);
  void connect (const SignalSlot& slot);
  void disconnect (int event);
  void disconnectAll ();
  
private:
  void run ();
  void updateEvent (int missedEvents);
    
  Glib::Thread *mThread;
  Glib::Mutex mEventMutex;
  //Glib::Mutex mWaitMutex;
  Glib::Cond mEventsInQueue;
  bool mRunning;
  StateMachine *mSM;
  std::multimap <int, SignalSignal*> mSignalList;
  SignalSignal mSignalBroadcast;
};

#endif // STATE_MACHINE_THREAD_H
