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
}

void ViewManager::setDataLoadDir(const std::string &dir)
{
  mDataLoadDir = dir;
}

Widget *ViewManager::getWidget(const std::string &view, const std::string &widget)
{
  View *viewObj = mViewList[view];
  Widget *widgetObj = NULL;
  
  if(viewObj)
  {
    widgetObj = viewObj->getWidget(widget);
  }

  return widgetObj;
}