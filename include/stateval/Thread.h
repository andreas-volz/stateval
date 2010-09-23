#ifndef THREAD_H
#define THREAD_H

/* STD */
#include <cassert>

/* PThreads */
#include <pthread.h>

namespace Threading {

class Mutex
{
  friend class Condition;
  pthread_mutex_t m_Mutex;

public:
  Mutex()
      : m_Mutex()
  {
    int rc = ::pthread_mutex_init(&m_Mutex, NULL);
    assert(0 == rc);
  }

  ~Mutex()
  {
    int rc = ::pthread_mutex_destroy(&m_Mutex);
    assert(0 == rc);
  }

  void lock()
  {
    int rc = ::pthread_mutex_lock(&m_Mutex);
    assert(0 == rc);
  }

  void unlock()
  {
    int rc = ::pthread_mutex_unlock(&m_Mutex);
    assert(0 == rc);
  }
}; // class Mutex

class MutexGrabber
{
  Mutex& m_Mutex;

public:
  MutexGrabber(Mutex& inMutex)
      : m_Mutex(inMutex)
  {
    m_Mutex.lock();
  }

  ~MutexGrabber()
  {
    m_Mutex.unlock();
  }

}; // class MutexGrabber

class Condition
{
  pthread_cond_t m_Condition;

public:
  Condition()
      : m_Condition()
  {
    int rc = ::pthread_cond_init(&m_Condition, NULL);
    assert(0 == rc);
  }

  ~Condition()
  {
    int rc = ::pthread_cond_destroy(&m_Condition);
    assert(0 == rc);
  }

  void signal()
  {
    int rc = ::pthread_cond_signal(&m_Condition);
    assert(0 == rc);
  }

  void wait(Mutex& inMutex)
  {
    int rc = ::pthread_cond_wait(&m_Condition, &inMutex.m_Mutex);
    assert(0 == rc);
  }

}; // class Condition

class Thread
{
  mutable Mutex m_AccessGuard;
  pthread_t m_ThreadHd;

  enum EState {eStopped, eStarting, eRunning, eStopping};
  EState m_State;

public:
  Thread()
      : m_AccessGuard()
      , m_ThreadHd(0)
      , m_State(eStopped)
  {}

  virtual ~Thread() {}

  void start()
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

  void cancel()
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

  void join()
  {
    pthread_join(m_ThreadHd, NULL);
  }

  bool isRunning() const
  {
    MutexGrabber grab(m_AccessGuard);

    return (eRunning == m_State);
  }


protected:
  virtual void run() = 0;
  virtual void signal_cancel() {}

private:
  static void* _run(void* inArg)
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
}; // class Thread

} // end namespace Threading

#endif // THREAD_H
