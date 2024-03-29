#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

/* Project */
#include "stateval/StateMachineAccessor.h"
#include "stateval/private/View.h"
#include "GUIThread.h"
#include "EdjeContext.h"
#include "EdjeView.h"
#include "EcoreDispatcher.h"
#include <Job.h>

/* STD */
#include <iostream>
#include <string>
#include <cstdlib>

using namespace std;
using efl::eo::instantiate;

GUIThread::GUIThread(const std::map <std::string, std::string> &params) :
  mLogger("stateval.plugins.viewmanager.edje.GUIThread"),
  mRunning(true),
  mEdjeView (NULL),
  mViewManagerParams(params),
  mWindow(NULL),
  mBackground(NULL),
  mBorderless(false),
  mTitle("StatEval Default Window"),
  mAlpha(false),
  mShaped(false),
  mViewCreated(false),
  mFullscreen(false)
{
  std::map <std::string, std::string>::const_iterator param_it;

  param_it = params.find ("width");
  if (param_it != params.end ())
  {
    mWindowSize.w = atoi (param_it->second.c_str ());
  }

  param_it = params.find ("height");
  if (param_it != params.end ())
  {
    mWindowSize.h = atoi (param_it->second.c_str ());
  }

  param_it = params.find ("borderless");
  if (param_it != params.end ())
  {
    if(param_it->second == "true")
    {
      mBorderless = true;
    }
  }

  param_it = params.find ("title");
  if (param_it != params.end ())
  {
    mTitle = param_it->second;    
  }

  param_it = params.find ("alpha");
  if (param_it != params.end ())
  {
    if(param_it->second == "true")
    {
      mAlpha = true;
    }
  }

  param_it = params.find ("shaped");
  if (param_it != params.end ())
  {
    if(param_it->second == "true")
    {
      mShaped = true;
    }
  }

  param_it = params.find ("fullscreen");
  if (param_it != params.end ())
  {
    if(param_it->second == "true")
    {
      mFullscreen = true;
    }
  }

}

GUIThread::~GUIThread()
{
  delete mViewFactoryDispatcher;
}

View *GUIThread::viewFactory (const std::string &dir, const std::map <std::string, std::string> &params)
{
  mViewParams = params;
  mDataLoadDir = dir;

  mMutexViewCreated.lock();
  mViewCreated = false;
  mViewFactoryDispatcher->emit();

  while(!mViewCreated)
  {
    mCondViewCreated.wait(mMutexViewCreated);
  }
  mMutexViewCreated.unlock();

  assert (mEdjeView);
  return mEdjeView;
}

Threading::Thread::EError GUIThread::start()
{
  Threading::Thread::EError error = Threading::Thread::start ();
  
  // synchronize startup - wait until EFL GUI is started
  mutexGUIStarted.lock ();
  condGUIStarted.wait (mutexGUIStarted);
  mutexGUIStarted.unlock ();
  
  return error;
}

static void
on_efl_ui_win_event_delete_request(void *data, const Efl_Event *event)
{
    //void info = event->info;
    //Eo *obj = event->object;
    //Data *d = data;
 
    /* event hander code */
  cout << "on_efl_ui_win_event_delete_request" << endl;
}

void GUIThread::run()
{
  // FIXME: use a better EFL C++ way for init handling
  elm_init(0, NULL);
  //mApp  = efl::App::app_main_get();

  mWindow = new efl::ui::Win(instantiate);
  assert (mWindow);
  
  mBackground = new efl::ui::Bg(instantiate, *mWindow);
  assert (mBackground);

  mViewFactoryDispatcher = new EcoreDispatcher();
  
  mViewFactoryDispatcher->signalDispatch.connect(sigc::mem_fun(this, &GUIThread::viewFactoryDispatched));
  
  mWindow->text_set(mTitle);
  mWindow->borderless_set(mBorderless);

  mBackground->hint_weight_set(EFL_GFX_HINT_EXPAND, EFL_GFX_HINT_EXPAND);
  mBackground->color_set(0, 0, 0, 1);

  mWindow->content_set(*mBackground);

  //FIXME: background handling broken since new EFL API
  //mBackground->hide();

  mWindow->size_set(mWindowSize);
  //mWindow->setAutoDel(false);
  mWindow->alpha_set(mAlpha);

  // FIXME: find out how to set shaped window mode => create an example for this
  //mWindow->setShaped(mShaped);

  mWindow->fullscreen_set(mFullscreen);


  // FIXME
  //efl_event_callback_add((Eo*)mWindow, EFL_UI_WIN_EVENT_DELETE_REQUEST, on_efl_ui_win_event_delete_request, NULL);
  
  // FIXME: the window delete handling just works with the new EFL API without this => need to find out why
  //mWindow->getEventSignal("delete,request")->connect(sigc::mem_fun(*this, &GUIThread::elm_quit));

  // FIXME
  /*mWindow->signal_callback_add("*", "*", [this] (auto&& signal, auto&&
    emission, auto&& source) {
    this->allFunc(signal, emission.c_str(), source.c_str()); 
  });*/

  Job startupJob;
  startupJob.signalCall.connect(sigc::mem_fun(this, &GUIThread::startupDispatched));
  startupJob.start();


  efl::ui::Layout *layout = new efl::ui::Layout(instantiate, *mWindow);
    
  // Enter the application main loop
  LOG4CXX_INFO(mLogger, "enter GUI mainloop");
  
  //mApp->run();
  // FIXME: use a better EFL C++ way for init handling
  elm_run();
}

void GUIThread::viewFactoryDispatched(int missedEvents)
{
  mContext.window = mWindow;
  mContext.resolution = mWindowSize;
  mContext.background = mBackground;
  
  mEdjeView = new EdjeView (&mContext, mDataLoadDir, mViewParams);
  mViewCreated = true;
    
  mMutexViewCreated.lock(); 
  mCondViewCreated.signal();
  mMutexViewCreated.unlock();
}

void GUIThread::elm_quit(/*Evasxx::Object &obj,*/ void *event_info)
{
  // TODO: exit concept! (finish state in stateval?)
  
  StateMachineAccessor &stateMachineAccessor(StateMachineAccessor::getInstance());
  
  // TODO: think about defining some common events (update/exit,...) in stateval lib 
  static const int EVENT_EXIT = stateMachineAccessor.findMapingEvent("EXIT");
    
  stateMachineAccessor.pushEvent(EVENT_EXIT);
  
  //Ecorexx::Application::quit();
  elm_shutdown();
  elm_exit();
}

/*!
 * This functions runs on the Ecore thread dispatched by the EcoreDispatcher
 */
void GUIThread::startupDispatched()
{
  // signal that EFL is started to EdjeViewManager
  mutexGUIStarted.lock();
  condGUIStarted.signal();
  mutexGUIStarted.unlock();
}
