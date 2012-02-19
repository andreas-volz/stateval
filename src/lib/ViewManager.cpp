#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

/* local */
#include "stateval/private/ViewManager.h"
#include "MemoryUtil.h"

/* STD */
#include <iostream>

using namespace std;

ViewManager::ViewManager() :
  mLogger("stateval.ViewManager")
{
}

ViewManager::~ViewManager()
{
  // free memory for all views
  delete_stl_container (mViewList);
  mViewList.clear ();       
}
