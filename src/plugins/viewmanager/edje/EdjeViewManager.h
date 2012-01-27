#ifndef EDJE_VIEW_MANAGER_H
#define EDJE_VIEW_MANAGER_H

/* STD */
#include <string>
/* pluxx */
#include <pluxx/Plugin.h>

/* local */
#include "Logger.h"
#include "EdjeView.h"
#include "EdjeContext.h"
#include "GUIThread.h"

/* stateval */
#include "stateval/private/ViewManager.h"

class EdjeViewManager : public ViewManager
{
public:
  EdjeViewManager(const std::map <std::string, std::string> &params);

  const std::string getType();

  const unsigned int getMajorVersion();

  const unsigned int getMinorVersion();

  View *loadView(const std::map <std::string, std::string> &params);

private:
  Logger mLogger;

  // FIXME: code heavy depends on initialization order!
  GUIThread mGUIThread;
  EdjeContext mContext;
};

#endif // EDJE_VIEW_MANAGER_H

