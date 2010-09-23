#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

/* Project */
#include "InputThread.h"
#include <stateval/StateMachineAccess.h>

/* STD */
#include <iostream>
#include <string>

using namespace std;

InputThread::InputThread (StateMachineAccess &smAccess) :
  mRunning (true),
  mSMAccess (&smAccess)
{
}

void InputThread::run ()
{
  // TODO: create a mutex around eventQueue?
  
  while (mRunning)
  {
    string input;  
    
    cout << "Waiting for command..." << endl << endl << endl;
    
    cin >> input;

    cout << "Typed: " << input << endl;
    
    mSMAccess->pushEvent (mSMAccess->findMapingEvent (input));
          
    if (input == "quit")
    {
      mRunning = false;
      cout << "Exiting..." << endl;
    }
  }
}
