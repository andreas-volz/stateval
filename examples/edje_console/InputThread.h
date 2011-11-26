#ifndef INPUT_THREAD_H
#define INPUT_THREAD_H

/* SIGC */
#include <sigc++/sigc++.h>

/* Project */
#include <stateval/private/Thread.h>

/* forward declarations */
class StateMachineAccessor;

class InputThread : public Threading::Thread
{
public:
  InputThread(StateMachineAccessor &smAccess);

private:
  void run();
  void updateEvent(int missedEvents);

  bool mRunning;
  StateMachineAccessor *mSMAccess;
};

#endif // INPUT_THREAD_H
