#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

/* Project */
#include "GUIThread.h"
#include <stateval/StateMachineAccessor.h>

/* STD */
#include <iostream>
#include <string>
#include <cstdlib>

using namespace std;

static const Eflxx::Size initialWindowSize(420, 480);

GUIThread::GUIThread() :
  mRunning(true),
  mApp (0, NULL),
  mWindow(Elmxx::Window::factory("window1", ELM_WIN_BASIC)),
  mBackground(Elmxx::Background::factory(*mWindow))
{
}

void GUIThread::run()
{
  StateMachineAccessor &stateMachineAccessor(StateMachineAccessor::getInstance());
  
  mWindow->setTitle("Simple stateval Test");

  mBackground->setWeightHintSize(EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
  mWindow->addObjectResize(*mBackground);

  mWindow->resize(initialWindowSize);
  mWindow->setAutoDel(true);

  mWindow->getEventSignal("delete,request")->connect(sigc::mem_fun(*this, &GUIThread::elm_quit));

  Ecorexx::Job startupJob;
  startupJob.signalCall.connect(sigc::mem_fun(this, &GUIThread::startupDispatched));
  startupJob.start();

  mWindow->show();
  mBackground->show();

  // Enter the application main loop
  mApp.run();
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
}
