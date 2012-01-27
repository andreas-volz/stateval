#ifndef TEXT_VIEW_MANAGER_H
#define TEXT_VIEW_MANAGER_H

/* STD */
#include <string>
/* pluxx */
#include <pluxx/Plugin.h>

/* local */
#include "Logger.h"
#include "TextView.h"
#include "InputThread.h"

/* stateval */
#include "stateval/private/ViewManager.h"

class TextViewManager : public ViewManager
{
public:
  TextViewManager(const std::map <std::string, std::string> &params);

  const std::string getType();

  const unsigned int getMajorVersion();

  const unsigned int getMinorVersion();

  View *loadView(const std::map <std::string, std::string> &params);

private:
  Logger mLogger;
  InputThread mThread;
};

#endif // TEXT_VIEW_MANAGER_H
