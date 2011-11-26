#ifndef THREAD_H
#define THREAD_H

/* PThreads */
#include <pthread.h>
#include <stdint.h>

namespace Threading
{

/** \brief
  *     C++ Abstraction of a mutex.
  *     For posix based OS this class uses posix mutex (pthread_mutex_t) for
  *     its implementation. This Mutex cooperates with the classes Thread and
  *     Condition.
  * \note
  *     For Win32 there is currently no implementation.
  *
  * \author Hans-Peter Dusel<hdusel@tangerine-soft.de>
  * \see #Threading::Condition
  * \see #Threading::Thread
  */
class Mutex
{
  friend class Condition;
  pthread_mutex_t m_Mutex;

public:

  enum EError
  {
    eErrorOK   = 0  //!< Denotes "no error".
    , eErrorLock      //!< Denotes "Error by locking the Mutex".
    , eErrorUnlock    //!< Denotes "Error by unlocking the Mutex".
    , eErrorLocked    //!< Denotes "Mutex is already locked".
  };

  /** Constructs and initializes this Mutex object but does not lock it.
    */
  Mutex();

  /** Destroys this Mutex object but does not unlock it.
    */
  ~Mutex();

  /** Locks this Mutex. If the Mutex is already locked then this call
    * blocks and waits for the other mutex to unlock.
    *
    * \return either #eErrorOK or #eErrorLock.
    *
    * \see unlock()
    * \see trylock()
    */
  EError lock();

  /** Tries to lock the Mutex. If the Mutex is already locked then this call
    * will return immediately with the error code eErrorLocked.
    *
    * \return either #eErrorOK, #eErrorLock or #eErrorLocked.
    *
    * \see #lock()
    * \see #unlock()
    */
  EError trylock();

  /** Unlock the Mutex which has previously been locked either by lock or tryLock().
    *
    * \return either #eErrorOK or #eErrorUnlock.
    *
    * \see #lock()
    * \see #trylock()
    */
  EError unlock();

// The assignment and copy ctor of these objects are intentionally not allowed!
private:
  Mutex(const Mutex &);
  Mutex &operator=(const Mutex &);

}; // class Mutex

/** \brief
  *     Automatically locks and unlocks a given mutex.
  *  This is a convenient class which locks a mutex on objects construction time and unlocks
  *  it again when the object is destroyed.
  *
  * \author Hans-Peter Dusel<hdusel@tangerine-soft.de>
  * \see #Threading::Mutex
  */
class MutexGrabber
{
  Mutex &m_Mutex; //!< The mutex to deal with.

public:
  /** Constructs a mutex grabber object and locks the assigned mutex.
    * \param inMutex The mutex to lock in the ctor and unlock in the dtor.
    */
  MutexGrabber(Mutex &inMutex);

  /** Destroys a mutex grabber object and unlocks the related mutex which has
    *  been locked previously in the ctor.
    */
  ~MutexGrabber();

// The assignment and copy ctor of these objects are intentionally not allowed!
private:
  MutexGrabber(const MutexGrabber &);
  MutexGrabber &operator=(const MutexGrabber &);
}; // class MutexGrabber

/** \brief
  *     C++ Abstraction of a condition Variable.
  *     For posix based OS this class uses posix conditions (pthread_cond_t) for
  *     its implementation. This Mutex cooperates with the class Mutex (see #wait(Mutex&)).
  * \note
  *     For Win32 there is currently no implementation.
  *
  * \author Hans-Peter Dusel<hdusel@tangerine-soft.de>
  * \see #Threading::Mutex
  * \see #Threading::Thread
  */
class Condition
{
  pthread_cond_t m_Condition;

public:
  Condition();
  ~Condition();

  void signal();
  void wait(Mutex &inMutex);

// The assignment and copy ctor of these objects are intentionally not allowed!
private:
  Condition(const Condition &);
  Condition &operator=(const Condition &);
}; // class Condition

/** \brief
  *     C++ Abstraction of a thread.
  *     For posix based OS this class uses posix threads (pthread_t).
  *     its implementation. This Thread may cooperate with the classes Mutex and Condition.
  *
  *     This is a pure virtual class so the implementor has to
  *     derive its own implementation from this class and has to implement the method
  *     virtual void run();
  *
  *     In addition the implementation has the freedom to implement the virtual class
  *     method #signal_cancel() which is supposed to give th concrete implementation a
  *     change to stop all actions after a #cancel() has been called.
  *
  * \note
  *     For Win32 there is currently no implementation.
  *
  * \author Hans-Peter Dusel<hdusel@tangerine-soft.de>
  * \see #Threading::Mutex
  * \see #Threading::Condition
  */
class Thread
{
  mutable Mutex m_AccessGuard; //!< The mutex which guards this thread agains concurrent accesses.
  pthread_t     m_ThreadHd;    //!< The posix thread handle for the thread implmentation.

  /** Each thread is in a certain state. These states are proposed to be used just internally. */
  enum EState
  {
    eStopped  //!< Denotes that the state is in Stopped state. It may be started next by calling #start().
    , eStarting //!< Denotes that the state is in Starting state. This is caused by #start().
    , eRunning  //!< Denotes that the state is in Running state hence the thread is executing #run()
    , eStopping //!< Denotes that the state is in Stopping state. This is caused by #cancel()
  };

  EState m_State; //!< The current state of the object

public:
  /** Various Error codes which belongs to threads. */
  enum EError
  {
    eErrorOK = 0    //!< Denotes "no error".
    , eErrorStart     //!< Denotes "Error to start the thread".
    , eErrorStop      //!< Denotes "Error to stop the thread".
    , eErrorJoin      //!< Denotes "Error to join the thread".
  };

  /** Constructs a Thread object but does *not* launch it.
    */
  Thread();

  /** Destroys this Thread object. The destructor even calls stop() in order to stop an
    * already running thread. Internally the thread is stopped state.
    */
  virtual ~Thread() {}

  /** Launches this Thread. The thread will be detached.
    * If the thread is already running then this call will return immediately without action.
    *
    * A running Thread may be stopped at any time by calling #cancel().
    *
    * @return Either one of #eErrorOK or #eErrorStart.
    *
    * \see #start()
    * \see #cancel()
    * \see #join()
    * \see #isRunning()
    * \see #signal_cancel()
    */
  EError start();

  /** Tries to stop this Thread. The thread will be joined.
    * If the thread is not already running then this call will return immediately without action.
    * A stopped Thread may be started again at any time by calling start().
    *
    * \note
    *     The implementation internally calls #signal_cancel(). Herein the implementation may
    *     take some actions to unblock (eventually) blocking conditions, which may prevent the thread
    *     to finish the run loop.
    *
    *     For example assume that a thread is waiting for incomming data within a message queue by
    *     applying a blocked call (e.g. receiveDataBlockedFromQueue())
    *
    *     Now the implementation may post a "dummyMessage" within #signal_cancel() in order
    *     to unblock the call which gives the thread a chance to finish.
    *
    * \return Either one of #eErrorOK or #eErrorStop.
    *
    * \see #start()
    * \see #cancel()
    * \see #join()
    * \see #isRunning()
    * \see #signal_cancel()
    */
  EError cancel();

  /** Tries to join this Thread. Joining means that the call blocks until the thread
    * reaches the end of the pure virtual run() method.
    *
    * @return Either one of #eErrorOK or #eErrorJoin.
    *
    * \see #start()
    * \see #cancel()
    * \see #join()
    * \see #isRunning()
    * \see #signal_cancel()
    */
  EError join();

  /** Queries if the thread is already running.
    *
    * \return true if running, false if stopped.
    * \note
    *     When a state is either in starting state (immediatelly after start())
    *     or in stopping state (signalled by cancel()) then this call returns false!
    *
    * \see #start()
    * \see #cancel()
    * \see #join()
    * \see #signal_cancel()
    */
  bool isRunning() const
  {
    MutexGrabber grab(m_AccessGuard);
    return (eRunning == m_State);
  }

// -----------------------------
// Static methods
// -----------------------------
  /** Relinquishes the Thread by a given time.
    *
    * \param inTimeMS The time to suspend the thread in milliseconds.
    */
  static void sleepMS(uint32_t inTimeMS);

protected:

  /** This is the only call which has to be implemented by a concrete implementation of a thread
    * object. This method will be called as soon as the thread is detached.
    */
  virtual void run() = 0;

  /** This (optional) method is supposed to take all actions to unblock blocking calls which may prevent the
    * thread to finish its run mehod. For details pls. refer the description for #cancel()
    *
    * \see #cancel()
    */
  virtual void signal_cancel() {}

private:
  /** The actual entry of a posix thread. This actually calls the pure virtual method run().
    * \param inArg this void* is assumed to be a Thread*.
    */
  static void *_run(void *inArg);

// Intentionally obj. assignment ad copy ctor of these objects are not allowed!
private:
  Thread(const Thread &);
  Thread &operator=(const Thread &);
}; // class Thread

} // end namespace Threading

#endif // THREAD_H
