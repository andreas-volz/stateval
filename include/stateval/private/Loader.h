#ifndef LOADER_H
#define LOADER_H

/* STD */
#include <string>
#include <vector>
#include <map>

/* local */
#include "State.h"
#include "Logger.h"
#include "ViewCache.h"
#include "ViewManager.h"
#include "stateval/Variable.h"

/* pluxx */
#include <pluxx/Plugin.h>

/* forward delarations */
class Context;
class Type;

class Loader : public pluxx::Plugin
{
public:
  Loader();
  virtual ~Loader();

  void setDataLoadDir(const std::string &dir);
  
  virtual bool load(const std::string &smDir) = 0;

  void addEvent(const std::string &event);

  void addState(State *state);

  void addAction(Action *action);

  void addVariable(const std::string &var, Variable &av);

  Variable *getVariable(const std::string &var);

  void changeVariable(const std::string &var, Variable &av);

  Widget *getWidget(const std::string &view, const std::string &widget);

  State *getInitialState();

  int findMapingEvent(const std::string &event);
  std::string findMapingEvent(int event);

  void start ();

protected:
  void loadViewManager(const std::string &viewmanagerPlugin, const std::map <std::string, std::string> &params);

  /* -> These data containers are basicly the complete statemachine data.
   *    This data is freed by the Loader destructor.
   *    So Loader plugins doesn't have to trouble about the basic data containers
   *    and only have to free their own temporary loaded containers...
   */
  std::vector <State *> mStateList;
  ViewManager *mViewManager;
  std::list <Action *> mActionList;
  std::map <std::string, Variable *> mVariableList;
  std::map <std::string, std::string> mViewManagerParams;

  // ... but no need to free this as it holds no allocated types...
  std::map <std::string, int> mEventList;
  std::vector <std::string> mEventListIndex;
  // <-
  ViewCache mViewCache;
  std::string mDataLoadDir;

private:
  Logger mLogger;
  int eventCounter;
};

#endif // LOADER_H

