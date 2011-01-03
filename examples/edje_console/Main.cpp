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

#if 0
void buidStateMachine (StateMachine &sm, Evasxx::Canvas &evas)
{
  // the following should happen from file format read...
  sm.addEvent ("HK_NAV");
  sm.addEvent ("HK_PHONE");
  sm.addEvent ("HK_TUNER");
  sm.addEvent ("HK_MEDIA");
  sm.addEvent ("HK_RETURN");
  sm.addEvent ("SK_1");
  sm.addEvent ("SK_2");
  sm.addEvent ("SK_3");
  sm.addEvent ("SK_4");
  sm.addEvent ("NAV_MAIN");
  sm.addEvent ("NAV_COUNTRY");
  sm.addEvent ("NAV_CITY");
  sm.addEvent ("NAV_STREET");
  sm.addEvent ("NAV_INFO");
  sm.addEvent ("TUNER_LIST");
  sm.addEvent ("TUNER_FREQ");
  sm.addEvent ("TEL_ARD");
  sm.addEvent ("TEL_DIAL");
  sm.addEvent ("MEDIA_LIST");
  sm.addEvent ("MEDIA_DETAIL");
  sm.addEvent ("MEDIA_TITLE_DETAILS");
  sm.addEvent ("MEDIA_CD_DETAILS");

  // Global Vaiables
  Bool *b1 = new Bool (true);

  GlobalVariables &global = GlobalVariables::instance ();
  global.addVariable ("tuner.available", *b1);

  // Views EDJE_FILE
  EdjeView *v1 = new EdjeView (searchEdjeFile("screens.edj"), "navigation_main"); // NAVI MAIN
  v1->addEventMapping (sm.findMapingEvent ("SK_1"), sm.findMapingEvent ("NAV_COUNTRY"));
  v1->addEventMapping (sm.findMapingEvent ("SK_2"), sm.findMapingEvent ("NAV_INFO"));

  EdjeView *v2 = new EdjeView (searchEdjeFile("screens.edj"), "navigation_country"); // NAVI COUNTRY INPUT
  v2->addEventMapping (sm.findMapingEvent ("SK_1"), sm.findMapingEvent ("NAV_CITY"));

  EdjeView *v3 = new EdjeView (searchEdjeFile("screens.edj"), "navigation_city"); // NAVI CITY INPUT
  v3->addEventMapping (sm.findMapingEvent ("SK_1"), sm.findMapingEvent ("NAV_STREET"));

  EdjeView *v4 = new EdjeView (searchEdjeFile("screens.edj"), "navigation_street"); // NAVI STREET INPUT
  v4->addEventMapping (sm.findMapingEvent ("SK_1"), sm.findMapingEvent ("NAV_MAIN"));

  EdjeView *v5 = new EdjeView (searchEdjeFile("screens.edj"), "tuner_list"); // TUNER LIST
  v5->addEventMapping (sm.findMapingEvent ("SK_1"), sm.findMapingEvent ("TUNER_FREQ"));

  EdjeView *v6 = new EdjeView (searchEdjeFile("screens.edj"), "tuner_frequenz"); // TUNER FREQ
  v6->addEventMapping (sm.findMapingEvent ("HK_RETURN"), sm.findMapingEvent ("TUNER_LIST"));

  EdjeView *v7 = new EdjeView (searchEdjeFile("screens.edj"), "navigation_info"); // NAV INFO
  v7->addEventMapping (sm.findMapingEvent ("HK_RETURN"), sm.findMapingEvent ("NAV_MAIN"));

  // HINT: root compound needs to be inserted at first!!!!
  CompoundState *s1 = new CompoundState (); // root compound - root has no parent!
  SimpleState *s2 = new SimpleState (s1); // root initial state - root is parent
  CompoundState *s3 = new CompoundState (s1); // nav compound - root is parent
  SimpleState *s13 = new SimpleState (s3); // nav initial state - nav compound is parent
  ViewState *s4 = new ViewState (s3, *v1); // nav main state - nav compound is parent
  ViewState *s5 = new ViewState (s3, *v2); // nav country input state - nav compound is parent
  ViewState *s6 = new ViewState (s3, *v3); // nav city input state - nav compound is parent
  ViewState *s7 = new ViewState (s3, *v4); // nav street input state - nav compound is parent
  CompoundState *s8 = new CompoundState (s1); // tuner compound - root compound is parent
  SimpleState *s9 = new SimpleState (s8); // tuner initial state - tuner compound is parent
  HistoryState *s14 = new HistoryState (s8); // tuner history state - tuner compound is parent
  ViewState *s10 = new ViewState (s8, *v5); // tuner list state - tuner compound is parent    
  ViewState *s11 = new ViewState (s8, *v6); // tuner freq state - tuner compound is parent
  ViewState *s12 = new ViewState (s3, *v7); // nav info state - nav compound is parent
  DecisionState *s15 = new DecisionState (s1); // tuner available decision state - root is parent
  
  Transition *s1_t1 = new Transition (s2); // implicit destination is root start state
  s1->addLeaveTransition (*s1_t1);
  Transition *s1_t2 = new Transition (s3, sm.findMapingEvent ("HK_NAV")); // destination is nav compound
  s1->addLeaveTransition (*s1_t2);
  Transition *s1_t3 = new Transition (s15, sm.findMapingEvent ("HK_TUNER")); // destination is tuner compound (s8)
  s1->addLeaveTransition (*s1_t3);

  Transition *s2_t1 = new Transition (s13); // destination is navi compound
  s2->addLeaveTransition (*s2_t1);

  Transition *s3_t1 = new Transition (s13); // implicit destination is navi start
  s3->addLeaveTransition (*s3_t1);

  Transition *s4_t1 = new Transition (s5, sm.findMapingEvent ("NAV_COUNTRY")); // destination is nav country
  s4->addLeaveTransition (*s4_t1);
  Transition *s4_t2 = new Transition (s12, sm.findMapingEvent ("NAV_INFO")); // destination is nav info
  s4->addLeaveTransition (*s4_t2);

  Transition *s5_t1 = new Transition (s6, sm.findMapingEvent ("NAV_CITY")); // destination is nav country
  s5->addLeaveTransition (*s5_t1);

  Transition *s6_t1 = new Transition (s7, sm.findMapingEvent ("NAV_STREET")); // destination is navi country
  s6->addLeaveTransition (*s6_t1);

  Transition *s7_t1 = new Transition (s4, sm.findMapingEvent ("NAV_MAIN")); // destination is navi main
  s7->addLeaveTransition (*s7_t1);
  
  Transition *s8_t1 = new Transition (s9); // implicit destination is tuner start
  s8->addLeaveTransition (*s8_t1);
  Transition *s8_t2 = new Transition (s10, sm.findMapingEvent ("TUNER_LIST")); // destination is tuner list
  s8->addLeaveTransition (*s8_t2);
  Transition *s8_t3 = new Transition (s11, sm.findMapingEvent ("TUNER_FREQ")); // destination is tuner freq
  s8->addLeaveTransition (*s8_t3);
  
  Transition *s9_t1 = new Transition (s14); // destination is tuner history
  s9->addLeaveTransition (*s9_t1);

  //s11->addEntryAction (new FireEventAction (sm, sm.findMapingEvent ("HK_NAV"))); // after entering fire new event
  s11->addExitAction (new Action ("trigger all events to stop a search...."));
  
  Transition *s12_t1 = new Transition (s4, sm.findMapingEvent ("NAV_MAIN")); // destination is nav main
  s12->addLeaveTransition (*s12_t1);
  
  Transition *s13_t1 = new Transition (s4); // destination is nav main
  s13->addLeaveTransition (*s13_t1);
  
  // tuner history
  s8->setHistory (s14);
  s14->changeTransition (s10);
  
  // tuner decision
  Transition *s15_t1 = new Transition (s8); // destination is tuner compound
  Bool *b2 = new Bool (true);
  Condition *c1 = new Condition ();
  c1->addComparison ("tuner.available", b2);
  std::pair <Condition*, Transition*> conTrans (c1, s15_t1);
  s15->addConditionTransition (conTrans);

  Transition *s16_t1 = new Transition (s7); // destination is nav street input
  Bool *b3  = new Bool (false);
  Condition *c2 = new Condition ();
  c2->addComparison ("tuner.available", b3);
  std::pair <Condition*, Transition*> conTrans2 (c2, s16_t1);
  s15->addConditionTransition (conTrans2);

  sm.addState (s1); // 0
  sm.addState (s2); // 1
  sm.addState (s3); // 2
  sm.addState (s4); // 3
  sm.addState (s5); // 4
  sm.addState (s6); // 5
  sm.addState (s7); // 6
  sm.addState (s8); // 7
  sm.addState (s9); // 8
  sm.addState (s10); // 9
  sm.addState (s11); // 10
  sm.addState (s12); // 11
  sm.addState (s13); // 12
  sm.addState (s14); // 13
  sm.addState (s15); // 14
  //
}
#endif 
