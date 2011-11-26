#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

/* STD */
#include <iostream>
#include <string>
#include <map>
#include <cassert>

/* local */
#include <stateval/stateval.h>
#include "searchFile.h"
#include "InputThread.h"
#include "Main.h"

/* log4cxx */
#ifdef HAVE_LOG4CXX
#include "log4cxx/logger.h"
#include "log4cxx/basicconfigurator.h"
#include "log4cxx/propertyconfigurator.h"
#include "log4cxx/helpers/exception.h"
#endif // HAVE_LOG4CXX

/* EFL */
#include <evasxx/Evasxx.h>
#include <edjexx/Edjexx.h>

/* stateval plugins */
#include "plugins/views/edje/EdjeContext.h"

using namespace std;

#ifdef HAVE_LOG4CXX
using namespace log4cxx;
using namespace log4cxx::helpers;
#endif // HAVE_LOG4CXX

//static const Eflxx::Size initialWindowSize (800, 600);
static const Eflxx::Size initialWindowSize(420, 480);

Main::Main(int argc, const char **argv) :
  mApp(argc, argv),
  mWindow(Elmxx::Window::factory("window1", ELM_WIN_BASIC)),
  mBackground(Elmxx::Background::factory(*mWindow))
{
#ifdef HAVE_LOG4CXX
  //BasicConfigurator::configure ();
  cout << "searchDataDir ():" << searchDataDir() << endl;
  PropertyConfigurator::configure(searchDataDir() + "/logging.prop");
#endif // HAVE_LOG4CXX

  mWindow->setTitle("Simple stateval Test");

  mBackground->setWeightHintSize(EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
  mWindow->addObjectResize(*mBackground);

  mWindow->resize(initialWindowSize);
  mWindow->setAutoDel(true);

  mWindow->getEventSignal("delete,request")->connect(sigc::mem_fun(*this, &Main::elm_quit));

  EdjeContext edjeContext(mWindow);
  edjeContext.setResolution(initialWindowSize);

  StateMachineAccessor &StateMachineAccessor(StateMachineAccessor::getInstance());
  StateMachineAccessor.load("smxml", searchDataDir() + "/mobile_smxml/mobile.smxml", &edjeContext);

  StateMachineAccessor.start();

  // create an input thread
  InputThread iThread(StateMachineAccessor);
  iThread.start();

  Ecorexx::Job startupJob;
  startupJob.signalCall.connect(sigc::mem_fun(this, &Main::startupDispatched));
  startupJob.start();

  mWindow->show();
  mBackground->show();

  // Enter the application main loop
  mApp.run();

  StateMachineAccessor::destroy();
  // <-- EFL
}

void Main::elm_quit(Evasxx::Object &obj, void *event_info)
{
  Ecorexx::Application::quit();
}

/*!
 * This functions runs on the Ecore thread dispatched by the EcoreDispatcher
 */
void Main::startupDispatched()
{
  StateMachineAccessor &StateMachineAccessor(StateMachineAccessor::getInstance());

  // inital event
  cout << "initial event" << endl;
  StateMachineAccessor.pushEvent("MAIN");
}

/* Main */
int main(int argc, const char **argv)
{
  Main mainobject(argc, argv);

  return 0;
}
