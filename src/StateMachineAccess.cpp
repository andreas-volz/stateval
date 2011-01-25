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

void StateMachineAccess::load (const std::string &loader, const std::string &file, Context *context)
{
  mSM = new StateMachine (loader);
  mSMThread = new StateMachineThread (*mSM);
  mSM->load (context, file);
}

void StateMachineAccess::start ()
{
  mSMThread->start (); 
  mSM->start ();
}

bool StateMachineAccess::isInitialized ()
{
  return (mSM && mSMThread);
}

void StateMachineAccess::pushEvent (int event)
{
  if (isInitialized ())
  {
    mSMThread->pushEvent (event);
  }
  else
  {
    assert (false);
  }
}

void StateMachineAccess::pushEvent (const std::string &event)
{
  if (isInitialized ())
  {
    mSMThread->pushEvent (event);
  }
  else
  {
    assert (false);
  }
}

int StateMachineAccess::findMapingEvent (const std::string &event)
{
  int ret = -1;
  if (isInitialized ())
  {
    ret = mSM->findMapingEvent (event);
  }
  else
  {
    assert (false);
  }
  
  return ret;
}

std::string StateMachineAccess::findMapingEvent (int event)
{
  string ret;
  if (isInitialized ())
  {
    return ret = mSM->findMapingEvent (event);
  }
  else
  {
    assert (false);
  }
  
  return "";
}

void StateMachineAccess::connect (int event, const SignalSlot& slot)
{
  if (isInitialized ())
  {
    mSMThread->connect (event, slot);
  }
  else
  {
    assert (false);
  }
}

void StateMachineAccess::connect (const std::string &event, const SignalSlot& slot)
{
  if (isInitialized ())
  {
    connect (findMapingEvent (event), slot);
  }
  else
  {
    assert (false);
  }
}

void StateMachineAccess::connect (const SignalSlot& slot)
{
  if (isInitialized ())
  {
    mSMThread->connect (slot);
  }
  else
  {
    assert (false);
  }
}

void StateMachineAccess::disconnect (int event)
{
  if (isInitialized ())
  {
    mSMThread->disconnect (event);
  }
  else
  {
    assert (false);
  }
}

void StateMachineAccess::disconnectAll ()
{
  if (isInitialized ())
  {
    mSMThread->disconnectAll ();
  }
  else
  {
    assert (false);
  }
}
