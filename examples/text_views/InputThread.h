#ifndef INPUT_THREAD_H
#define INPUT_THREAD_H

/* SIGC */
#include <sigc++/sigc++.h>

/* Project */
#include <stateval/Thread.h>

/* forward declarations */
class StateMachineAccess;

class InputThread : public Threading::Thread
{
public:
  InputThread (StateMachineAccess &smAccess);

private:
  void run ();
  void updateEvent (int missedEvents);
  
  bool mRunning;
  StateMachineAccess *mSMAccess;
};

#endif // INPUT_THREAD_H
