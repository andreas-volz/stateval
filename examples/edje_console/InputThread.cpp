#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

/* Project */
#include "InputThread.h"
#include <stateval/StateMachineAccess.h>

/* STD */
#include <iostream>
#include <string>
#include <map>

using namespace std;

InputThread::InputThread (StateMachineAccess &smAccess) :
  mRunning (true),
  mSMAccess (&smAccess)
{
  mThread = Glib::Thread::create (sigc::mem_fun (*this, &InputThread::run), false);
}

void InputThread::run ()
{
  map <string, int> keyMap;
  keyMap["HK_NAV"] = mSMAccess->findMapingEvent ("HK_NAV");
  keyMap["HK_PHONE"] = mSMAccess->findMapingEvent ("HK_PHONE");
  keyMap["HK_TUNER"] = mSMAccess->findMapingEvent ("HK_TUNER");
  keyMap["HK_MEDIA"] = mSMAccess->findMapingEvent ("HK_MEDIA");
  keyMap["HK_RETURN"] = mSMAccess->findMapingEvent ("HK_RETURN");
  keyMap["SK_1"] = mSMAccess->findMapingEvent ("SK_1");
  keyMap["SK_2"] = mSMAccess->findMapingEvent ("SK_2");
  keyMap["SK_3"] = mSMAccess->findMapingEvent ("SK_3");
  keyMap["SK_4"] = mSMAccess->findMapingEvent ("SK_4");
  
  // TODO: create a mutex around eventQueue?
  
  while (mRunning)
  {
    string input;  
    
    cout << "Waiting for command..." << endl << endl << endl;
    
    cin >> input;

    cout << "Typed: " << input << endl;
    
    map <string,int>::iterator iter = keyMap.find(input);
    if( iter != keyMap.end() )
    {
      int &inputEvent = iter->second;
      cout << "push element: " << inputEvent << endl;
      mSMAccess->pushEvent (inputEvent);
    }
          
    if (input == "quit")
    {
      mRunning = false;
      cout << "Exiting..." << endl;
    }
  }
}
