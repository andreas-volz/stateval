#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

/* STD */
#include <iostream>
#include <cassert>

/* Project */
#include "stateval/Loader.h"
#include "stateval/CompoundState.h"
#include "stateval/HistoryState.h"
#include "stateval/DecisionState.h"
#include "stateval/ViewState.h"
// TODO: Later provide some sort of plugin mechanism...
#ifdef HAVE_EFL
#include "stateval/EdjeView.h"
#endif
#include "stateval/TextView.h"
#include "stringUtil.h"

using namespace std;

Loader::Loader () :
  eventCounter (0)
{
}

Loader::~Loader ()
{
}

void Loader::addEvent (const std::string &event)
{
  mEventList[event] = eventCounter;
    
  ++eventCounter;
}

void Loader::addState (State *state)
{
  mStateList.push_back (state);
}

void Loader::addView (View *view)
{
  mViewList.push_back (view);
}

State *Loader::getInitialState ()
{
  return *mStateList.begin ();
}

int Loader::findMapingEvent (const std::string &event)
{
  map <string,int>::iterator iter = mEventList.find(event);
  if (iter != mEventList.end())
  {
    int &mapEvent = iter->second;
    //cout << "map event: " << iter->first << " : " << iter->second << endl;
    return mapEvent;
  }

  cerr << "StateMachine::findMapingEvent: try to find not existing event: " << event << endl;

  return -1;
}
