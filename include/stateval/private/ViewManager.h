#ifndef VIEW_MANAGER_H
#define VIEW_MANAGER_H

/* STD */
#include <string>
/* pluxx */
#include <pluxx/Plugin.h>

/* local */
#include "Logger.h"
#include "View.h"

class ViewManager : public pluxx::Plugin
{
public:
  ViewManager();
  virtual ~ViewManager();

  virtual View *loadView(const std::map <std::string, std::string> &params) = 0;

protected:
  std::list <View*> mViewList;
  
private:
  Logger mLogger;
};

#endif // VIEW_MANAGER_H
