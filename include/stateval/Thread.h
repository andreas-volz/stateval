#ifndef THREAD_H
#define THREAD_H

/* PThreads */
#include <pthread.h>

namespace Threading {

class Mutex
{
  friend class Condition;
  pthread_mutex_t m_Mutex;

public:
  Mutex();
  ~Mutex();

  void lock();
  void unlock();
}; // class Mutex

class MutexGrabber
{
  Mutex& m_Mutex;

public:
  MutexGrabber(Mutex& inMutex);
  ~MutexGrabber();
}; // class MutexGrabber

class Condition
{
  pthread_cond_t m_Condition;

public:
   Condition();
  ~Condition();

  void signal();
  void wait(Mutex& inMutex);
}; // class Condition

class Thread
{
  mutable Mutex m_AccessGuard;
  pthread_t     m_ThreadHd;

  enum EState {eStopped, eStarting, eRunning, eStopping};
  EState m_State;

public:
  Thread();

  virtual ~Thread(){}

  void start();
  void cancel();
  void join();

  bool isRunning() const
  {
    MutexGrabber grab(m_AccessGuard);
    return (eRunning == m_State);
  }

protected:
  virtual void run() = 0;
  virtual void signal_cancel() {}

private:
  static void* _run(void* inArg);
}; // class Thread

} // end namespace Threading

#endif // THREAD_H
