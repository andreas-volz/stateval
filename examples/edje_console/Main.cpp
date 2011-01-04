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

/* EFL */
#include <evasxx/Evasxx.h>
#include <edjexx/Edjexx.h>

/* stateval plugins */
#include "plugins/views/edje/EdjeContext.h"

using namespace std;

static const Eflxx::Size initialWindowSize (800, 600);

Main::Main (int argc, const char **argv) :
  mApp (argc, argv, "Simple stateval Test"),
  mWindow (initialWindowSize),
  mBackgroundRect (mWindow.getCanvas())
{
  mWindow.deleteRequestSignal.connect (sigc::mem_fun (this, &Main::hideWindow));
  mWindow.resizeSignal.connect (sigc::mem_fun (this, &Main::resizeWindow));
  mWindow.setAlpha (true); // enable transparency for the window

  // setup background
  mBackgroundRect.setColor (Eflxx::Color (0, 0, 0, 0)); // set a transparent background
  mBackgroundRect.resize (initialWindowSize);
  mBackgroundRect.setFocus (true);
  mBackgroundRect.show ();
    
  EdjeContext edjeContext (mWindow.getCanvas());
  edjeContext.setResolution (initialWindowSize);
  
  StateMachine sm ("smxml");
  
  sm.load (&edjeContext, searchDataDir () + "/edje_smxml/test.smxml");
  
  // fill statemachine with data
  //buidStateMachine (sm, evas);
  
  StateMachineThread smThread (sm);
  
  smThread.start ();
  
  StateMachineAccess &stateMachineAccess (StateMachineAccess::instance ());
  stateMachineAccess.init (sm, smThread);

  // create an input thread
  InputThread iThread (stateMachineAccess);
  iThread.start ();
  
  sm.init ();
  
  // inital event
  // TODO Ecorexx::Job
  stateMachineAccess.pushEvent ("HK_NAV");

  mWindow.show();

  // Enter the application main loop
  mApp.exec();

  // <-- EFL
}

void Main::quit ()
{
  Ecorexx::Application::quit();
}

void Main::hideWindow (const Ecorexx::EvasWindow &win)
{
  cout << "hide" << endl;
  quit ();
}

void Main::resizeWindow (const Ecorexx::EvasWindow &win)
{
  //GraphicContext &graphicContext (GraphicContext::instance ());
  
  const Eflxx::Size winSize (mWindow.geometry ().size ());
  
  //graphicContext.setResolution (winSize);
  mBackgroundRect.resize (winSize);
}

/* Main */
int main (int argc, const char **argv)
{
  Main mainobject (argc, argv);
    
  return 0;
}
