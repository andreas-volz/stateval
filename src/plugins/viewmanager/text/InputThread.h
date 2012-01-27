#ifndef INPUT_THREAD_H
#define INPUT_THREAD_H

/* Project */
#include <stateval/private/Thread.h>

/* forward declarations */
class StateMachineAccessor;

class InputThread : public Threading::Thread
{
public:
  InputThread();

private:
  void run();
  void updateEvent(int missedEvents);

  bool mRunning;
};

#endif // INPUT_THREAD_H
