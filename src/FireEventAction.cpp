#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

/* Project */
#include "../include/stateval/FireEventAction.h"
#include "../include/stateval/StateMachine.h"

/* STD */
#include <iostream>

using namespace std;

FireEventAction::FireEventAction (StateMachine &sm, int event) :
  mEvent (event),
  mSM (&sm)
{
}

void FireEventAction::run () const
{
  mSM->pushEvent (mEvent);
}
