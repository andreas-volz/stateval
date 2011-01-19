#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

/* local */
#include "stateval/ViewState.h"
#include "Logger.h"

/* STD */
#include <iostream>

using namespace std;

static Logger logger ("stateval.ViewState");

ViewState::ViewState (CompoundState *parentState, View &view) :
  SimpleState (parentState),
  mView (&view)
{
}

bool ViewState::hasView ()
{
  return mView;
}

View *ViewState::getView ()
{
  return mView;
}

void ViewState::mapEvent (int &inOutEvent)
{
  if (hasView ())
  {
    mView->mapEvent (inOutEvent);
  }
}

void ViewState::beforeTransitionCode ()
{
  if (hasView ())
  {
    getView ()->unrealize ();
  }
  else
  {
    LOG4CXX_DEBUG (logger, "State has no view attached");
  }
}

void ViewState::afterTransitionCode ()
{
  changeHistory ();
  
  if (hasView ())
  {
    getView ()->realize ();
  }
  else
  {
    LOG4CXX_DEBUG (logger, "State has no view attached");
  }
}

void ViewState::pushEvent (int event)
{
  if (hasView ())
  {
    getView ()->pushEvent (event);
  }
}
