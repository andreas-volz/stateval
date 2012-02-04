#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

/* local */
#include "stateval/private/StateMachineLoop.h"
#include "stateval/private/StateMachine.h"
#include "MemoryUtil.h"

/* STD */
#include <iostream>
#include <string>
#include <map>
#include <cassert>

using namespace std;

StateMachineLoop::StateMachineLoop(StateMachine &sm) :
  mLogger("stateval.StateMachineLoop"),
  mEventMutex(),
  mEventsInQueue(),
  mSM(&sm),
  mSignalList(),
  mSignalBroadcast()
{
}

StateMachineLoop::~StateMachineLoop()
{
  LOG4CXX_TRACE(mLogger, "~StateMachineLoop");

//  cancel();

  LOG4CXX_TRACE(mLogger, "~StateMachineLoop (canceled)");

//  join();

  LOG4CXX_TRACE(mLogger, "~StateMachineLoop (joined)");
}

void StateMachineLoop::start()
{
  LOG4CXX_TRACE(mLogger, "+StateMachineLoop::start ()");
//  Thread::start();
  LOG4CXX_TRACE(mLogger, "-StateMachineLoop::start ()");
}

void StateMachineLoop::signal_cancel() // from thread
{
  mEventsInQueue.signal();
}

void StateMachineLoop::run()
{
  LOG4CXX_TRACE(mLogger, "+run");

  while (/*isRunning()*/ 1)
  {
    LOG4CXX_TRACE(mLogger, "+run::running while");

    mEventMutex.lock();

    // this waiting loop runs until someone pushed an event to the event queue
    while (!mSM->hasEvents())
    {
      LOG4CXX_TRACE(mLogger, "!mSM->eventQueue.empty()");
      // here is the point the loop waits if no event is in the event queue
      mEventsInQueue.wait(mEventMutex);
      // FIXME: commented out since thread redesign
      // TODO: define cancel method!
      /*if (!isRunning())
      {
        mEventMutex.unlock();
        LOG4CXX_TRACE(mLogger, "!isRunning()");
        return;
      }*/
    }
    LOG4CXX_TRACE(mLogger, "mSM->eventQueue.empty()");

    int event = mSM->getNextEvent();
    mEventMutex.unlock();

    LOG4CXX_DEBUG(mLogger, "+EventQueue size: " << mSM->getEventCount());

    mSM->evaluateState(event);

    // pop element after working
    LOG4CXX_DEBUG(mLogger, "-EventQueue size: " << mSM->getEventCount());

    // emit event signals
    multimap <int, SignalSignal *>::iterator findResult = mSignalList.find(event);
    multimap <int, SignalSignal *>::iterator lastElement = mSignalList.upper_bound(event);

    if (findResult != mSignalList.end())
    {
      // emit also multible signals...
      for (; findResult != lastElement; ++findResult)
      {
        LOG4CXX_DEBUG(mLogger, "call event '" << event << "' to app");
        SignalSignal *signal = (*findResult).second;
        signal->emit(event);
      }
    }
    LOG4CXX_TRACE(mLogger, "-run::running while");

    // emit the signal broadcast
    // this is e.g. useful to dispatch signals to another thread
    mSignalBroadcast.emit(event);

    mEventMutex.lock();
    mSM->popEvent();
    mEventMutex.unlock();

    LOG4CXX_TRACE(mLogger, "-run");
  }
}

void StateMachineLoop::pushEvent(int event)
{
  // called from async GUI mainloop thread, so locking the queue is needed
  mEventMutex.lock();
  mSM->pushEvent(event);
  mEventsInQueue.signal();
  mEventMutex.unlock();
}

void StateMachineLoop::pushEvent(const std::string &event)
{
  pushEvent(mSM->findMapingEvent(event));
}

void StateMachineLoop::connect(int event, const SignalSlot &slot)
{
  SignalSignal *signal = new SignalSignal();
  mSignalList.insert(pair <int, SignalSignal *> (event, signal));

  signal->connect(slot);
}

void StateMachineLoop::connect(const SignalSlot &slot)
{
  mSignalBroadcast.connect(slot);
}

void StateMachineLoop::disconnect(int event)
{
  // delete event signals
  multimap <int, SignalSignal *>::iterator findResult = mSignalList.find(event);
  multimap <int, SignalSignal *>::iterator lastElement = mSignalList.upper_bound(event);

  if (findResult != mSignalList.end())
  {
    // delete all connected handlers
    for (; findResult != lastElement; ++findResult)
    {
      SignalSignal *signal = findResult->second;
      delete signal;
      mSignalList.erase(findResult);
    }
  }
}

void StateMachineLoop::disconnectAll()
{
  delete_stl_container(mSignalList);
}
