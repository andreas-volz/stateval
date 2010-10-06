#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

/* Project */
#include "stateval/StateMachineAccess.h"

/* STD */
#include <iostream>

using namespace std;

StateMachineAccess &StateMachineAccess::instance ()
{
  static StateMachineAccess _instance;
  return _instance;
}

StateMachineAccess::~StateMachineAccess ()
{
  cout << "~StateMachineAccess" << endl;
}

void StateMachineAccess::init (StateMachine &sm, StateMachineThread &smThread)
{
  mSM = &sm;
  mSMThread = &smThread;
}

void StateMachineAccess::pushEvent (int event)
{
  mSMThread->pushEvent (event);
}

void StateMachineAccess::pushEvent (const std::string &event)
{
  mSMThread->pushEvent (event);
}

int StateMachineAccess::findMapingEvent (const std::string &event)
{
  return mSM->findMapingEvent (event);
}

void StateMachineAccess::connect (int event, const SignalSlot& slot)
{
  mSMThread->connect (event, slot);
}

void StateMachineAccess::connect (std::string event, const SignalSlot& slot)
{
  connect (findMapingEvent (event), slot);
}

void StateMachineAccess::connect (const SignalSlot& slot)
{
  mSMThread->connect (slot);
}

void StateMachineAccess::disconnect (int event)
{
  mSMThread->disconnect (event);
}

void StateMachineAccess::disconnectAll ()
{
  mSMThread->disconnectAll ();
}
