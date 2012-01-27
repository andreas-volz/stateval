#ifndef GUI_THREAD_H
#define GUI_THREAD_H

/* EFL */
#include <evasxx/Evasxx.h>
#include <ecorexx/Ecorexx.h>
#include <edjexx/Edjexx.h>
#include <elementaryxx/Elementaryxx.h>

/* Project */
#include <stateval/private/Thread.h>

/* forward declarations */
class StateMachineAccessor;

class GUIThread : public Threading::Thread
{
public:
  GUIThread();

private:  
  void run();
  void updateEvent(int missedEvents);
  void elm_quit(Evasxx::Object &obj, void *event_info);
  void startupDispatched();
  
  bool mRunning;
  Elmxx::Application mApp;

  // FIXME: code heavy depends on initialization order! 
  
public:
  Elmxx::Window *mWindow;

private:
  Elmxx::Background *mBackground;
};

#endif // GUI_THREAD_H

