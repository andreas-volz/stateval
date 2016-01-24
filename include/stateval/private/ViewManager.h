#ifndef VIEW_MANAGER_H
#define VIEW_MANAGER_H

/* STD */
#include <string>
/* pluxx */
#include <pluxx/Plugin.h>

/* local */
#include "Logger.h"
#include "View.h"
#include "stateval/Variable.h"

class ViewManager : public pluxx::Plugin
{
public:
  ViewManager();
  virtual ~ViewManager();

  virtual View *loadView(const std::string &name, const std::map <std::string, std::string> &params) = 0;

  Widget *getWidget(const std::string &view, const std::string &widget);

protected:
  std::map <std::string, View*> mViewList;
  
private:
  Logger mLogger;
};

#endif // VIEW_MANAGER_H
