#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

/* local */
#include "EdjeViewManager.h"

/* STD */
#include <iostream>

using namespace std;

static const char *type = "ViewManager";
static const unsigned int major_version = 1;
static const unsigned int minor_version = 1;

EdjeViewManager::EdjeViewManager(const std::map <std::string, std::string> &params) :
  mLogger("stateval.EdjeViewManager"),
  mGUIThread (params)
{ 
  mGUIThread.start();

  // wait until EFL GUI is started
  // TODO: wait/lock in mGUIThread.start() wrapper?
  mGUIThread.mutexGUIStarted.lock ();
  mGUIThread.condGUIStarted.wait (mGUIThread.mutexGUIStarted);
  mGUIThread.mutexGUIStarted.unlock ();
  cout << "GUI initialized" << endl;
}

View *EdjeViewManager::loadView(const std::map <std::string, std::string> &params)
{
  View *edjeview = mGUIThread.viewFactory (params);

  mViewList.push_back (edjeview);

  return edjeview;
}

void EdjeViewManager::start ()
{
  
}

//////////////////////
// Plugin stuff below
//////////////////////

const std::string EdjeViewManager::getType()
{
  return type;
}

const unsigned int EdjeViewManager::getMajorVersion()
{
  return major_version;
}

const unsigned int EdjeViewManager::getMinorVersion()
{
  return minor_version;
}

/*****************************/
/* Plugin needed C functions */
/*****************************/

PLUGIN_EXPORT EdjeViewManager *plugin_create(const std::map <std::string, std::string> &params)
{
  return new EdjeViewManager(params);
}

PLUGIN_EXPORT void plugin_destroy(ViewManager *plugin)
{
  delete plugin;
}

PLUGIN_EXPORT const char *get_plugin_type()
{
  return type;
}

PLUGIN_EXPORT unsigned int get_plugin_major_version()
{
  return major_version;
}
