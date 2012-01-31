#ifndef EDJE_VIEW_MANAGER_H
#define EDJE_VIEW_MANAGER_H

/* STD */
#include <string>
/* pluxx */
#include <pluxx/Plugin.h>

/* local */
#include "GUIThread.h"

/* stateval */
#include "stateval/private/ViewManager.h"
#include "stateval/private/Logger.h"

class EdjeViewManager : public ViewManager
{
public:
  EdjeViewManager(const std::map <std::string, std::string> &params);

  const std::string getType();

  const unsigned int getMajorVersion();

  const unsigned int getMinorVersion();

  View *loadView(const std::map <std::string, std::string> &params);

  void start ();

private:
  Logger mLogger;  // first private variable

  GUIThread mGUIThread;
};

#endif // EDJE_VIEW_MANAGER_H

