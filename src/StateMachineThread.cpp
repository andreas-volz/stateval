#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

/* Project */
#include "../include/stateval/StateMachineThread.h"
#include "../include/stateval/StateMachine.h"

/* STD */
#include <iostream>
#include <string>
#include <map>
#include <cassert>

using namespace std;

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
  cout << "~StateMachineThread" << endl;
  cancel ();
}

void StateMachineThread::start ()
{
  cout << "+StateMachineThread::start ()" << endl;
  Thread::start();
  cout << "-StateMachineThread::start ()" << endl;
}

void StateMachineThread::signal_cancel() // from thread
{
    mEventsInQueue.signal ();
}

void StateMachineThread::run ()
{
  cout << "StateMachineThread::running" << endl;
  
  while (isRunning())
  {
    cout << "StateMachineThread::running while" << endl;

    mEventMutex.lock ();
    if (mSM->eventQueue.empty())
    {
      mEventsInQueue.wait (mEventMutex);
    }
    else
    {
      int event = mSM->eventQueue.front();
      mEventMutex.unlock ();
      
      cout << "EventQueue size: " << mSM->eventQueue.size () << endl;

      mSM->evaluateState (event);
      
      // pop element after working
      cout << "EventQueue size: " << mSM->eventQueue.size () << endl;
      cout << "pop element" << endl;
      cout << endl;

			// emit event signals
      multimap <int, SignalSignal*>::iterator findResult = mSignalList.find (event);
      multimap <int, SignalSignal*>::iterator lastElement = mSignalList.upper_bound (event);
      
      if (findResult != mSignalList.end ())
      {
        // emit also multible signals...
	      for ( ; findResult != lastElement; ++findResult)
        {
          cout << "call event '" << event << "' to app" << endl;
          SignalSignal *signal = (*findResult).second;
          signal->emit (event);
        }
      }

      // emit the signal broadcast
      // this is e.g. usefull to dispatch signals to another thread
      mSignalBroadcast.emit (event);
      
      mEventMutex.lock ();
      mSM->popEvent ();
    }
    mEventMutex.unlock ();
    
    cout << "StateMachineThread running in the background..." << endl;
  }
}

void StateMachineThread::pushEvent (int event)
{
  mEventMutex.lock ();
  mSM->pushEvent (event);
  mEventMutex.unlock ();
  
  mEventsInQueue.signal ();
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
      SignalSignal *signal = (*findResult).second;
      delete signal;
        
     // TODO from hdusel: Forget to remove the element from the multimap?
     // Discuss this with Andreas.
    }
  }
}

void StateMachineThread::disconnectAll ()
{
	for (std::multimap <int, SignalSignal*>::iterator s_it = mSignalList.begin ();
	     s_it != mSignalList.end ();
	     ++s_it)
	{
		int event = (*s_it).first;
		SignalSignal *signal = (*s_it).second;

		delete signal;
	}
}
