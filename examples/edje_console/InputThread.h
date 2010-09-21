#ifndef INPUT_THREAD_H
#define INPUT_THREAD_H

/* GLIB */
#include <sigc++/sigc++.h>
#include <glibmm/thread.h>

/* Project */
#include <stateval/EcoreDispatcher.h>

/* forward declarations */
class StateMachineAccess;

// TODO: rename to StateMachineThread
class InputThread
{
public:
  InputThread (StateMachineAccess &smAccess);

private:
  void run ();
  void updateEvent (int missedEvents);
    
  Glib::Thread *mThread;
  //Glib::Mutex mEventMutex;
  bool mRunning;
  StateMachineAccess *mSMAccess;
};

#endif // INPUT_THREAD_H
