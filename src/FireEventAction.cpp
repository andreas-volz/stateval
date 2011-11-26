#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

/* Project */
#include "stateval/private/FireEventAction.h"
#include "stateval/StateMachineAccessor.h"

/* STD */
#include <iostream>

using namespace std;

FireEventAction::FireEventAction(int event) :
  mEvent(event)
{
}

void FireEventAction::run() const
{
  StateMachineAccessor &StateMachineAccessor(StateMachineAccessor::getInstance());
  StateMachineAccessor.pushEvent(mEvent);
}
