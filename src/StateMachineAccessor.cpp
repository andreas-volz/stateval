#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

/* local */
#include "stateval/StateMachineAccessor.h"
#include "stateval/private/StateMachine.h"
#include "stateval/private/StateMachineThread.h"
#include "StateMachineAccessorPImpl.h"

/* STD */
#include <iostream>

using namespace std;

/* static member variable initialization */
StateMachineAccessor *StateMachineAccessor::mInstance = NULL;

StateMachineAccessor::StateMachineAccessor() :
  mPImpl(new StateMachineAccessorPImpl())
{

}

StateMachineAccessor::~StateMachineAccessor()
{
  delete mPImpl;
}

void StateMachineAccessor::destroy()
{
  if (mInstance)
  {
    delete mInstance;
  }
  mInstance = NULL;
}

StateMachineAccessor &StateMachineAccessor::getInstance()
{
  if (!mInstance)
  {
    mInstance = new StateMachineAccessor();
  }
  return *mInstance;
}

// TODO: unload before load?
void StateMachineAccessor::load(const std::string &loader, const std::string &file, Context *context)
{
  mPImpl->mSM = new StateMachine(loader);
  mPImpl->mSMThread = new StateMachineThread(*mPImpl->mSM);
  mPImpl->mSM->load(context, file);
}

void StateMachineAccessor::start()
{
  mPImpl->mSMThread->start();
  mPImpl->mSM->start();
}

bool StateMachineAccessor::isInitialized()
{
  return (mPImpl->mSM && mPImpl->mSMThread);
}

void StateMachineAccessor::pushEvent(int event)
{
  if (isInitialized())
  {
    mPImpl->mSMThread->pushEvent(event);
  }
  else
  {
    assert(false);
  }
}

void StateMachineAccessor::pushEvent(const std::string &event)
{
  if (isInitialized())
  {
    mPImpl->mSMThread->pushEvent(event);
  }
  else
  {
    assert(false);
  }
}

void StateMachineAccessor::addVariable(const std::string &var, AbstractVariable &av)
{
  // TODO: locking
  mPImpl->mSM->addVariable(var, av);
}

AbstractVariable *StateMachineAccessor::getVariable(const std::string &var)
{
  // TODO: locking
  mPImpl->mSM->getVariable(var);
}

void StateMachineAccessor::changeVariable(const std::string &var, AbstractVariable &av)
{
  // TODO: locking
  mPImpl->mSM->changeVariable(var, av);
}

int StateMachineAccessor::findMapingEvent(const std::string &event)
{
  int ret = -1;
  if (isInitialized())
  {
    ret = mPImpl->mSM->findMapingEvent(event);
  }
  else
  {
    assert(false);
  }

  return ret;
}

std::string StateMachineAccessor::findMapingEvent(int event)
{
  string ret;
  if (isInitialized())
  {
    return ret = mPImpl->mSM->findMapingEvent(event);
  }
  else
  {
    assert(false);
  }

  return "";
}

void StateMachineAccessor::connect(int event, const SignalSlot &slot)
{
  if (isInitialized())
  {
    mPImpl->mSMThread->connect(event, slot);
  }
  else
  {
    assert(false);
  }
}

void StateMachineAccessor::connect(const std::string &event, const SignalSlot &slot)
{
  if (isInitialized())
  {
    connect(findMapingEvent(event), slot);
  }
  else
  {
    assert(false);
  }
}

void StateMachineAccessor::connect(const SignalSlot &slot)
{
  if (isInitialized())
  {
    mPImpl->mSMThread->connect(slot);
  }
  else
  {
    assert(false);
  }
}

void StateMachineAccessor::disconnect(int event)
{
  if (isInitialized())
  {
    mPImpl->mSMThread->disconnect(event);
  }
  else
  {
    assert(false);
  }
}

void StateMachineAccessor::disconnectAll()
{
  if (isInitialized())
  {
    mPImpl->mSMThread->disconnectAll();
  }
  else
  {
    assert(false);
  }
}
