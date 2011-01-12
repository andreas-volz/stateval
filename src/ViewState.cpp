#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

/* Project */
#include "stateval/ViewState.h"

/* STD */
#include <iostream>

using namespace std;

ViewState::ViewState (State *parentState, View &view) :
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
    cout << "State has no view attached" << endl; 
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
    cout << "State has no view attached" << endl; 
  }
}

void ViewState::pushEvent (int event)
{
  if (hasView ())
  {
    getView ()->pushEvent (event);
  }
}
