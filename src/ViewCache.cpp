#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

/* local */
#include "stateval/private/ViewCache.h"

/* STD */
#include <algorithm>

bool operator == (const ViewSpec &viewSpec1, const ViewSpec &viewSpec2)
{
  return viewSpec1.view == viewSpec2.view;
}

using namespace std;

ViewCache::ViewCache () :
  mLogger ("stateval.ViewCache")
{

}

void ViewCache::setUnrealizeViewList (std::list <ViewSpec> unrealizeViewList)
{
  LOG4CXX_TRACE (mLogger, "setUnrealizeViewList");
  
  mUnrealizeViewList = unrealizeViewList;
}

void ViewCache::setRealizeViewList (std::list <ViewSpec> realizeViewList)
{
  LOG4CXX_TRACE (mLogger, "setRealizeViewList");
  
  list <ViewSpec> mReallyUnrealizeViewList;
  list <ViewSpec> mReallyRealizeViewList;

  // search all views that like to unrealize if maybe one of them is in the next
  // realize list and then don't unrealize it
  for (std::list <ViewSpec>::iterator v_it = mUnrealizeViewList.begin ();
       v_it != mUnrealizeViewList.end ();
       ++v_it)
  {
    ViewSpec &unrealizeViewSpec = *v_it;
    //View *unrealizeView = viewSpec.view;

    list <ViewSpec>::iterator found_view_it = find (realizeViewList.begin (), realizeViewList.end (), unrealizeViewSpec);

    // if view isn't longer in active list mark it to unrealize
    if (found_view_it == realizeViewList.end ())
    {
      LOG4CXX_TRACE (mLogger, "push back unrealize view");
      mReallyUnrealizeViewList.push_back (*found_view_it);
    }
  }

  // search all views that like to realize which one are really new and add those to a realize list
  for (std::list <ViewSpec>::iterator v_it = realizeViewList.begin ();
       v_it != realizeViewList.end ();
       ++v_it)
  {
    ViewSpec &realizeViewSpec = *v_it;
    //View *realizeView = viewSpec.view;

    list <ViewSpec>::iterator found_view_it = find (mUnrealizeViewList.begin (), mUnrealizeViewList.end (), realizeViewSpec);

    // if view isn't longer in active list mark it to unrealize
    if (found_view_it == mUnrealizeViewList.end ())
    {
      LOG4CXX_TRACE (mLogger, "push back realize view");
      mReallyRealizeViewList.push_back (realizeViewSpec);
    }
  }

  // unrealize all before really marked views
  for (std::list <ViewSpec>::iterator v_it = mReallyUnrealizeViewList.begin ();
       v_it != mReallyUnrealizeViewList.end ();
       ++v_it)
  {
    ViewSpec &reallyUnrealizeViewSpec = *v_it;
    View *reallyUnrealizeView = reallyUnrealizeViewSpec.view;

    LOG4CXX_TRACE (mLogger, "reallyUnrealizeView->unrealize ()");
    reallyUnrealizeView->unrealize ();
  }

  // unrealize all before really marked views
  for (std::list <ViewSpec>::iterator v_it = mReallyRealizeViewList.begin ();
       v_it != mReallyRealizeViewList.end ();
       ++v_it)
  {
    ViewSpec &reallyRealizeViewSpec = *v_it;
    View *reallyRealizeView = reallyRealizeViewSpec.view;

    LOG4CXX_TRACE (mLogger, "reallyRealizeView->realize ()");
    reallyRealizeView->setLayer (reallyRealizeViewSpec.layer);
    reallyRealizeView->realize ();
  }
}
