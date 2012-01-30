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

static const Eflxx::Size initialWindowSize(800, 480);

GUIThread::GUIThread() :
  mRunning(true),
  mEdjeView (NULL)
{

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

void GUIThread::run()
{
  mApp = new Elmxx::Application (0, NULL);
  assert (mApp);

  mWindow = Elmxx::Window::factory("window1", ELM_WIN_BASIC);
  assert (mWindow);
  
  mBackground = Elmxx::Background::factory(*mWindow);
  assert (mBackground);

  // TODO: destroy at end
  mViewFactoryDispatcher = new EcoreDispatcher();
  
  mViewFactoryDispatcher->signalDispatch.connect(sigc::mem_fun(this, &GUIThread::viewFactoryDispatched));
  
  //StateMachineAccessor &stateMachineAccessor(StateMachineAccessor::getInstance());
  
  mWindow->setTitle("Simple stateval Test");
    
  mBackground->setWeightHintSize(EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
  mWindow->addObjectResize(*mBackground);

  mWindow->resize(initialWindowSize);
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
  mContext.resolution = initialWindowSize;
  
  cout << "new edje view" << endl;
  mEdjeView = new EdjeView (&mContext, mViewParams);

  mCondViewCreated.signal ();
}

void GUIThread::elm_quit(Evasxx::Object &obj, void *event_info)
{
  Ecorexx::Application::quit();
}

/*!
 * This functions runs on the Ecore thread dispatched by the EcoreDispatcher
 */
void GUIThread::startupDispatched()
{
  StateMachineAccessor &StateMachineAccessor(StateMachineAccessor::getInstance());

  // inital event
  cout << "initial event" << endl;
  StateMachineAccessor.pushEvent("MAIN");
  //StateMachineAccessor.pushEvent("HK_NAV");

  // signal that EFL is started to EdjeViewManager 
  condGUIStarted.signal ();
}
