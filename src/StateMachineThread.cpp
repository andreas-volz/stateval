#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

/* local */
#include "stateval/private/StateMachineThread.h"
#include "stateval/private/StateMachine.h"
#include "Logger.h"
#include "MemoryUtil.h"

/* STD */
#include <iostream>
#include <string>
#include <map>
#include <cassert>

using namespace std;

static Logger logger ("stateval.StateMachineThread");

StateMachineThread::StateMachineThread (StateMachine &sm) :
  Threading::Thread(),
  mEventMutex(),
  mEventsInQueue(),
  mSM(&sm),
  mSignalList(),
  mSignalBroadcast()
{
}

StateMachineThread::~StateMachineThread ()
{
  LOG4CXX_TRACE (logger, "~StateMachineThread");

  
  cancel ();

  LOG4CXX_TRACE (logger, "~StateMachineThread (canceled)");
  
  join ();

  LOG4CXX_TRACE (logger, "~StateMachineThread (joined)");
}

void StateMachineThread::start ()
{
  LOG4CXX_TRACE (logger, "+StateMachineThread::start ()");
  Thread::start();
  LOG4CXX_TRACE (logger, "-StateMachineThread::start ()");
}

void StateMachineThread::signal_cancel() // from thread
{
  mEventsInQueue.signal ();
}

void StateMachineThread::run ()
{
  LOG4CXX_TRACE (logger, "+run");
  
  while (isRunning())
  {
    LOG4CXX_TRACE (logger, "+run::running while");

    mEventMutex.lock ();

    // this waiting loop runs until someone pushed an event to the event queue
    while (!mSM->hasEvents ())
    {
      LOG4CXX_TRACE (logger, "!mSM->eventQueue.empty()");
      // here is the point the loop waits if no event is in the event queue
      mEventsInQueue.wait (mEventMutex);
      if (!isRunning())
      {
          mEventMutex.unlock ();
          LOG4CXX_TRACE (logger, "!isRunning()");
          return;
      }
    }
    LOG4CXX_TRACE (logger, "mSM->eventQueue.empty()");

    int event = mSM->getNextEvent ();
    mEventMutex.unlock ();
  
    LOG4CXX_DEBUG (logger, "+EventQueue size: " << mSM->getEventCount ());

    mSM->evaluateState (event);
  
    // pop element after working
    LOG4CXX_DEBUG (logger, "-EventQueue size: " << mSM->getEventCount ());

        // emit event signals
    multimap <int, SignalSignal*>::iterator findResult = mSignalList.find (event);
    multimap <int, SignalSignal*>::iterator lastElement = mSignalList.upper_bound (event);
  
    if (findResult != mSignalList.end ())
    {
      // emit also multible signals...
      for ( ; findResult != lastElement; ++findResult)
      {
        LOG4CXX_DEBUG (logger, "call event '" << event << "' to app");
        SignalSignal *signal = (*findResult).second;
        signal->emit (event);
      }
    }
    LOG4CXX_TRACE (logger, "-run::running while");

    // emit the signal broadcast
    // this is e.g. useful to dispatch signals to another thread
    mSignalBroadcast.emit (event);
  
    mEventMutex.lock ();
    mSM->popEvent ();
    mEventMutex.unlock ();
    
    LOG4CXX_TRACE (logger, "-run");
  }
}

void StateMachineThread::pushEvent (int event)
{
  mEventMutex.lock ();
  mSM->pushEvent (event);
  mEventsInQueue.signal ();
  mEventMutex.unlock ();
}

void StateMachineThread::pushEvent (const std::string &event)
{
  pushEvent (mSM->findMapingEvent (event));
}

void StateMachineThread::connect (int event, const SignalSlot& slot)
{
  SignalSignal* signal = new SignalSignal();
	mSignalList.insert (pair <int, SignalSignal*> (event, signal));

  signal->connect (slot);
}

void StateMachineThread::connect (const SignalSlot& slot)
{
  mSignalBroadcast.connect (slot);
}

void StateMachineThread::disconnect (int event)
{
	// delete event signals
  multimap <int, SignalSignal*>::iterator findResult = mSignalList.find (event);
  multimap <int, SignalSignal*>::iterator lastElement = mSignalList.upper_bound (event);
  
  if (findResult != mSignalList.end ())
  {
    // delete all connected handlers
    for ( ; findResult != lastElement; ++findResult)
    {
      SignalSignal *signal = findResult->second;
      delete signal;
      mSignalList.erase (findResult);
    }
  }
}

void StateMachineThread::disconnectAll ()
{
  delete_stl_container (mSignalList);
}
