#ifndef STATE_MACHINE_H
#define STATE_MACHINE_H

/* STD */
#include <vector>
#include <queue>

/* local */
#include "State.h"
#include "Loader.h"
#include "Logger.h"
#include "stateval/Variable.h"

/* forward declarations */

// the functions for the state machine aren't synchronized. Use StateMachineAccessor instead!!!
class StateMachine
{
public:
  StateMachine(const std::string &loaderPlugin);
  virtual ~StateMachine();

  void start();

  void setDataLoadDir(const std::string &dir);
  
  bool load(const std::string &smDir);

  void pushEvent(int event);

  void popEvent();

  void evaluateState(int &inOutEvent);

  int findMapingEvent(const std::string &event);
  std::string findMapingEvent(int event);

  bool hasEvents();
  unsigned int getEventCount();
  int getNextEvent();

  void addVariable(const std::string &var, Variable &av);

  Variable *getVariable(const std::string &var);

  void changeVariable(const std::string &var, Variable &av);

  Widget *getWidget(const std::string &view, const std::string &widget);

  static const int EMPTY_EVENT = -1; 

private:
  Logger mLogger;
  State *searchHierarchie(int event);
  bool walkDown(int event);

  std::queue <int> eventQueue;
  State *mActiveState;
  Loader *mLoader;
  bool mSMInit;
};

#endif // STATE_MACHINE_H
