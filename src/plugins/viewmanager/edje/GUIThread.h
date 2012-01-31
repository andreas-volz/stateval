#ifndef GUI_THREAD_H
#define GUI_THREAD_H

/* EFL */
#include <evasxx/Evasxx.h>
#include <ecorexx/Ecorexx.h>
#include <edjexx/Edjexx.h>
#include <elementaryxx/Elementaryxx.h>

/* stateval */
#include "stateval/private/Thread.h"
#include "stateval/private/Logger.h"

/* local */
#include "EdjeContext.h"

/* forward declarations */
class StateMachineAccessor;
class View;
class EcoreDispatcher;

class GUIThread : public Threading::Thread
{
public:
  GUIThread(const std::map <std::string, std::string> &params);
  ~GUIThread();

  View *viewFactory (const std::map <std::string, std::string> &params);

  Threading::Thread::EError start();

private:  
  void run();
  void updateEvent(int missedEvents);
  void elm_quit(Evasxx::Object &obj, void *event_info);
  void startupDispatched();
  void viewFactoryDispatched(int missedEvents);

  Logger mLogger; // first private variable
  
  bool mRunning;
  Elmxx::Application *mApp;
  EcoreDispatcher *mViewFactoryDispatcher;

  Threading::Condition mCondViewCreated;
  Threading::Mutex mMutexViewCreated;

  Threading::Condition condGUIStarted;
  Threading::Mutex mutexGUIStarted;
  
  EdjeContext mContext;
  View *mEdjeView;

  // synchronized by mViewFactoryDispatcher
  std::map <std::string, std::string> mViewParams;

  std::map <std::string, std::string> mViewManagerParams;

  Eflxx::Size mWindowSize;
    
  Elmxx::Window *mWindow;

  Elmxx::Background *mBackground;
};

#endif // GUI_THREAD_H

