#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

/* local */
#include "TextViewManager.h"

/* STD */
#include <iostream>

using namespace std;

static const char *type = "ViewManager";
static const unsigned int major_version = 1;
static const unsigned int minor_version = 1;

TextViewManager::TextViewManager(const std::map <std::string, std::string> &params) :
  mLogger("stateval.TextViewManager")
{
  mThread.start();
}

View *TextViewManager::loadView(const std::map <std::string, std::string> &params)
{
  TextView *textview = new TextView (params);

  mViewList.push_back (textview);

  return textview;
}

//////////////////////
// Plugin stuff below
//////////////////////

const std::string TextViewManager::getType()
{
  return type;
}

const unsigned int TextViewManager::getMajorVersion()
{
  return major_version;
}

const unsigned int TextViewManager::getMinorVersion()
{
  return minor_version;
}

/*****************************/
/* Plugin needed C functions */
/*****************************/

PLUGIN_EXPORT TextViewManager *plugin_create(Context *context, const std::map <std::string, std::string> &params)
{
  return new TextViewManager(params);
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