#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

/* Project */
#include "stateval/FireEventAction.h"
#include "stateval/StateMachineAccess.h"

/* STD */
#include <iostream>

using namespace std;

FireEventAction::FireEventAction (int event) :
  mEvent (event)
{
}

void FireEventAction::run () const
{
  StateMachineAccess &stateMachineAccess (StateMachineAccess::instance ());
  stateMachineAccess.pushEvent (mEvent);
}
