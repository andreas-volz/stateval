#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

/* local */
#include "stateval/StateMachineAccessor.h"
#include "stateval/private/StateMachine.h"
#include "stateval/private/StateMachineLoop.h"
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
void StateMachineAccessor::load(const std::string &loader, const std::string &file)
{
  mPImpl->mSM = new StateMachine(loader);
  mPImpl->mSMLoop = new StateMachineLoop(*mPImpl->mSM);
  mPImpl->mSM->load(file);
}

void StateMachineAccessor::run()
{
  mPImpl->mSM->start();
  mPImpl->mSMLoop->start();
  mPImpl->mSMLoop->run();
}

bool StateMachineAccessor::isInitialized()
{
  return (mPImpl->mSM && mPImpl->mSMLoop);
}

void StateMachineAccessor::pushEvent(int event)
{
  if (isInitialized())
  {
    mPImpl->mSMLoop->pushEvent(event);
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
    mPImpl->mSMLoop->pushEvent(event);
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
  return mPImpl->mSM->getVariable(var);
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
    mPImpl->mSMLoop->connect(event, slot);
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
    mPImpl->mSMLoop->connect(slot);
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
    mPImpl->mSMLoop->disconnect(event);
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
    mPImpl->mSMLoop->disconnectAll();
  }
  else
  {
    assert(false);
  }
}
