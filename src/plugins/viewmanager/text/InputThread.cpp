#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

/* Project */
#include "InputThread.h"
#include <stateval/StateMachineAccessor.h>

/* STD */
#include <iostream>
#include <string>
#include <cstdlib>

using namespace std;

InputThread::InputThread() :
  mRunning(true)
{
}

void InputThread::run()
{
  StateMachineAccessor &stateMachineAccessor(StateMachineAccessor::getInstance());
  
  while (mRunning)
  {
    string input;

    cout << "Waiting for command..." << endl << endl << endl;

    cin >> input;

    cout << "Typed: " << input << endl;

    stateMachineAccessor.pushEvent(stateMachineAccessor.findMapingEvent(input));

    if (input == "quit")
    {
      mRunning = false;
      cout << "Exiting..." << endl;
      exit(0);
    }
  }
}
