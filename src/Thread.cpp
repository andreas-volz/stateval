// Thread.cpp

#include "stateval/Thread.h"

/* STD */
#include <cassert>
#include <unistd.h>
#include <stdio.h>
#include <errno.h>

namespace Threading {

// ===============================================================================================
// = class Mutex
// ===============================================================================================
Mutex::Mutex()
: m_Mutex()
{
  const int rc(::pthread_mutex_init(&m_Mutex, NULL));
  assert(0 == rc);
}

Mutex::~Mutex()
{
  const int rc(::pthread_mutex_destroy(&m_Mutex));
  assert(0 == rc);
}

Mutex::EError Mutex::lock()
{
  int rc = ::pthread_mutex_lock(&m_Mutex);
  assert(0 == rc);
  return (0 == rc) ? eErrorOK : eErrorLock;
}

Mutex::EError Mutex::trylock()
{
  const int rc(::pthread_mutex_trylock(&m_Mutex));
  assert(EINVAL != rc);
  return (0     == rc) ? eErrorOK : 
         (EBUSY == rc) ? eErrorLocked : eErrorLock;
}

Mutex::EError Mutex::unlock()
{
  const int rc(::pthread_mutex_unlock(&m_Mutex));
  assert(0 == rc);
  return (0 == rc) ? eErrorOK : eErrorUnlock;
}

// ===============================================================================================
// = class MutexGrabber
// ===============================================================================================
MutexGrabber::MutexGrabber(Mutex& inMutex)
: m_Mutex(inMutex)
{
  m_Mutex.lock();
}

MutexGrabber::~MutexGrabber()
{
  m_Mutex.unlock();
}

// ===============================================================================================
// = class Condition
// ===============================================================================================
Condition::Condition()
: m_Condition()
{
  const int rc(::pthread_cond_init(&m_Condition, NULL));
  assert(0 == rc);
}

Condition::~Condition()
{
  const int rc(::pthread_cond_destroy(&m_Condition));
  assert(0 == rc);
}

void Condition::signal()
{
  const int rc(::pthread_cond_signal(&m_Condition));
  assert(0 == rc);
}

void Condition::wait(Mutex& inMutex)
{
  int rc = ::pthread_cond_wait(&m_Condition, &inMutex.m_Mutex);
  assert(0 == rc);
}

// ===============================================================================================
// = class Condition
// ===============================================================================================
// public
Thread::Thread()
: m_AccessGuard()
, m_ThreadHd(0)
, m_State(eStopped)
{}

Thread::EError Thread::start()
{
  MutexGrabber grab(m_AccessGuard);
  if (eStopped == m_State)
  {
    m_State = eStarting;
    const int rc(::pthread_create(&m_ThreadHd, NULL, _run, this));
    assert(0==rc);

    // Was the cthread created successfully?
    if (rc != 0)
    {
      // No it didn't :-(
      m_State = eStopped; // cancel the starting state on failure
      return Thread::eErrorStart; // return 'failure'.
    }
  }
  return Thread::eErrorOK; // return 'success'.
}

Thread::EError Thread::cancel()
{
  //MutexGrabber grab(m_AccessGuard);
  if (eRunning == m_State)
  {
    m_State = eStopping;
    //m_AccessGuard.unlock();
    
    signal_cancel();
    
    const Thread::EError err(join());
    assert(eStopped == m_State);
    //m_AccessGuard.lock();

    if (eErrorOK != err)
    {
      m_State = Thread::eStopped;
      return Thread::eErrorStop;
    }
  }
  //m_AccessGuard.unlock();
  return Thread::eErrorOK;
}

Thread::EError Thread::join()
{
  const int rc(::pthread_join(m_ThreadHd, NULL));
  return (0 == rc) ? Thread::eErrorOK : Thread::eErrorJoin;
}

// public, static
void Thread::sleepMS(uint32_t inTimeMS)
{
  ::usleep(1000 * inTimeMS);
}

// Thread - static, private
void* Thread::_run(void* inArg)
{
  Thread* inst = reinterpret_cast<Thread*> (inArg);
  inst->m_AccessGuard.lock();
  inst->m_State = eRunning;
  inst->m_AccessGuard.unlock();

  inst->run();
    
  inst->m_AccessGuard.lock();
  inst->m_State = eStopped;
  inst->m_AccessGuard.unlock();
  return NULL;
}

} // end namespace Threading
