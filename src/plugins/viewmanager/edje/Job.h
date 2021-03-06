#ifndef ECORE_JOB_H
#define ECORE_JOB_H

/* EFL */
#include <Ecore.h>

/* SIGC */
#include <sigc++/sigc++.h>

class Job
{
public:
  /*!
   * Add a job to the event queue.
   */
  Job ();
  
  /*!
   * Delete a queued job that has not yet been executed.
   */
  virtual ~Job ();
  
  /*!
   * Starts the job
   */
  void start ();
  
  /*!
   * Cancels the job if not yet runned
   */
  void cancel ();
  
  sigc::signal <void> signalCall;
  
private:
  static void callback (void *data);
  
private:
  Ecore_Job *mJob;
  bool mCalled;
};

#endif // ECORE_JOB_H
