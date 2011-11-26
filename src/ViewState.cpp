#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

/* local */
#include "stateval/private/ViewState.h"
#include "stateval/private/ViewCache.h"

/* STD */
#include <iostream>
#include <cassert>

using namespace std;

ViewState::ViewState(CompoundState *parentState, ViewCache *viewCache) :
  SimpleState(parentState),
  mViewCache(viewCache),
  mLogger("stateval.ViewState")
{
}

ViewState::~ViewState()
{
}

bool ViewState::hasView()
{
  mViewList.size();
}

void ViewState::mapEvent(int &inOutEvent)
{
  int event = inOutEvent;

  for (std::list <ViewSpec>::iterator v_it = mViewList.begin();
       v_it != mViewList.end();
       ++v_it)
  {
    ViewSpec &viewSpec = *v_it;
    View *view = viewSpec.view;

    // this maps all events from all views, if conflicting mappings exist, then
    // the top most event view mapping wins
    view->mapEvent(event);
  }

  inOutEvent = event;
}

void ViewState::addView(View &view, int layer)
{
  ViewSpec viewSpec;
  viewSpec.view = &view;
  viewSpec.layer = layer;
  mViewList.push_back(viewSpec);
}

void ViewState::beforeTransitionCode()
{
  assert(hasView());

  mViewCache->setUnrealizeViewList(mViewList);
}

void ViewState::afterTransitionCode()
{
  assert(hasView());

  changeHistory();

  mViewCache->setRealizeViewList(mViewList);
}

void ViewState::pushEvent(int event)
{
  for (std::list <ViewSpec>::iterator v_it = mViewList.begin();
       v_it != mViewList.end();
       ++v_it)
  {
    ViewSpec &viewSpec = *v_it;
    View *view = viewSpec.view;
    view->pushEvent(event);
  }
}
