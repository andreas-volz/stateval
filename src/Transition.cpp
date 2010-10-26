#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

/* Project */
#include "stateval/Transition.h"
#include "stateval/State.h"

Transition::Transition (State *state) :
  mEndState (state),
  mEvent (NoEvent)
{

}

Transition::Transition (State *state, int event) :
  mEndState (state),
  mEvent (event)
{

}

Transition::~Transition ()
{
}

int Transition::getEvent () const
{
  return mEvent;
}

State *Transition::getEndState () const
{
  return mEndState;
}

void Transition::setEndState (State *state)
{
  mEndState = state;
}
