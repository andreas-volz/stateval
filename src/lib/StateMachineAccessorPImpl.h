#ifndef STATE_MACHINE_ACCESSOR_PIMPL_H
#define STATE_MACHINE_ACCESSOR_PIMPL_H

#include <string>
#include "stateval/private/Thread.h"

struct StateMachineAccessorPImpl
{
  StateMachine *mSM;
  StateMachineLoop *mSMLoop;
  Threading::Mutex mMutex;
};

#endif // STATE_MACHINE_ACCESSOR_PIMPL_H
