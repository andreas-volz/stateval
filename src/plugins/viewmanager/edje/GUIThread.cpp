#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

/* Project */
#include "stateval/private/View.h"
#include "GUIThread.h"
#include <stateval/StateMachineAccessor.h>
#include "EdjeContext.h"
#include "EdjeView.h"
#include "EcoreDispatcher.h"

/* STD */
#include <iostream>
#include <string>
#include <cstdlib>

using namespace std;

GUIThread::GUIThread(const std::map <std::string, std::string> &params) :
  mLogger("stateval.plugins.viewmanager.GUIThread"),
  mRunning(true),
  mEdjeView (NULL),
  mViewManagerParams(params),
  mWindow(NULL),
  mBackground(NULL)
{
  std::map <std::string, std::string>::const_iterator param_it;

  // TODO: think about more generic value parser from XML
  param_it = params.find ("width");
  if (param_it != params.end ())
  {
    mWindowSize.width (atoi (param_it->second.c_str ()));
  }

  param_it = params.find ("height");
  if (param_it != params.end ())
  {
    mWindowSize.height (atoi (param_it->second.c_str ()));
  }  
}

GUIThread::~GUIThread()
{
  delete mViewFactoryDispatcher;
}

View *GUIThread::viewFactory (const std::map <std::string, std::string> &params)
{
  mViewParams = params;

  mViewFactoryDispatcher->emit();
  
  mMutexViewCreated.lock();  
  mCondViewCreated.wait(mMutexViewCreated);
  mMutexViewCreated.unlock();

  cout << "return" << endl;
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

void GUIThread::run()
{
  mApp = new Elmxx::Application (0, NULL);
  assert (mApp);

  mWindow = Elmxx::Window::factory("window1", ELM_WIN_BASIC);
  assert (mWindow);
  
  mBackground = Elmxx::Background::factory(*mWindow);
  assert (mBackground);

  mViewFactoryDispatcher = new EcoreDispatcher();
  
  mViewFactoryDispatcher->signalDispatch.connect(sigc::mem_fun(this, &GUIThread::viewFactoryDispatched));
  
  mWindow->setTitle("StatEval Default Window");
    
  mBackground->setWeightHintSize(EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
  mWindow->addObjectResize(*mBackground);

  mWindow->resize(mWindowSize);
  mWindow->setAutoDel(true);
  mWindow->setAlpha(true);
  
  mWindow->getEventSignal("delete,request")->connect(sigc::mem_fun(*this, &GUIThread::elm_quit));

  Ecorexx::Job startupJob;
  startupJob.signalCall.connect(sigc::mem_fun(this, &GUIThread::startupDispatched));
  startupJob.start();

  mWindow->show();
  mBackground->show();
  
  // Enter the application main loop
  cout << "enter mainloop" << endl;
  mApp->run();
}

void GUIThread::viewFactoryDispatched(int missedEvents)
{
  mContext.window = mWindow;
  mContext.resolution = mWindowSize;
  
  cout << "new edje view" << endl;
  mEdjeView = new EdjeView (&mContext, mViewParams);

  mCondViewCreated.signal ();
}

void GUIThread::elm_quit(Evasxx::Object &obj, void *event_info)
{
  // TODO: exit concept! (finite state in stateval?)
  Ecorexx::Application::quit();
}

/*!
 * This functions runs on the Ecore thread dispatched by the EcoreDispatcher
 */
void GUIThread::startupDispatched()
{
  // signal that EFL is started to EdjeViewManager 
  condGUIStarted.signal ();
}
