#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

/* Project */
#include "stateval/StateMachineAccessor.h"

/* STD */
#include <iostream>

using namespace std;

StateMachineAccessor &StateMachineAccessor::getInstance ()
{
  static StateMachineAccessor _instance;
  return _instance;
}

void StateMachineAccessor::load (const std::string &loader, const std::string &file, Context *context)
{
  mSM = new StateMachine (loader);
  mSMThread = new StateMachineThread (*mSM);
  mSM->load (context, file);
}

void StateMachineAccessor::start ()
{
  mSMThread->start (); 
  mSM->start ();
}

bool StateMachineAccessor::isInitialized ()
{
  return (mSM && mSMThread);
}

void StateMachineAccessor::pushEvent (int event)
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

void StateMachineAccessor::pushEvent (const std::string &event)
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

int StateMachineAccessor::findMapingEvent (const std::string &event)
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

std::string StateMachineAccessor::findMapingEvent (int event)
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

void StateMachineAccessor::connect (int event, const SignalSlot& slot)
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

void StateMachineAccessor::connect (const std::string &event, const SignalSlot& slot)
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

void StateMachineAccessor::connect (const SignalSlot& slot)
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

void StateMachineAccessor::disconnect (int event)
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

void StateMachineAccessor::disconnectAll ()
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
