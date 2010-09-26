// Thread.cpp

#include "stateval/Thread.h"

/* STD */
#include <cassert>
#include <unistd.h>
#include <stdio.h>

namespace Threading {

// ===============================================================================================
// = class Mutex
// ===============================================================================================
Mutex::Mutex()
: m_Mutex()
{
  int rc = ::pthread_mutex_init(&m_Mutex, NULL);
  assert(0 == rc);
}

Mutex::~Mutex()
{
  int rc = ::pthread_mutex_destroy(&m_Mutex);
  assert(0 == rc);
}

void Mutex::lock()
{
  int rc = ::pthread_mutex_lock(&m_Mutex);
  assert(0 == rc);
}

void Mutex::unlock()
{
  int rc = ::pthread_mutex_unlock(&m_Mutex);
  assert(0 == rc);
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
  int rc = ::pthread_cond_init(&m_Condition, NULL);
  assert(0 == rc);
}

Condition::~Condition()
{
  int rc = ::pthread_cond_destroy(&m_Condition);
  assert(0 == rc);
}

void Condition::signal()
{
  int rc = ::pthread_cond_signal(&m_Condition);
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

void Thread::start()
{
  m_AccessGuard.lock();
  if (eStopped == m_State)
  {
    m_State = eStarting;
    int rc = ::pthread_create(&m_ThreadHd, NULL, _run, this);
    assert(0==rc);
    if (rc != 0)
    {
      m_State = eStopped; // cancel the starting state on failure
    }
  }
  m_AccessGuard.unlock();
}

void Thread::cancel()
{
  m_AccessGuard.lock();
  if (eRunning == m_State)
  {
    m_State = eStopping;
    m_AccessGuard.unlock();
    
    signal_cancel();
    join();
    
    m_AccessGuard.lock();
    
    assert(eStopped == m_State);
  }
  m_AccessGuard.unlock();
}

void Thread::join()
{
  ::pthread_join(m_ThreadHd, NULL);
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
