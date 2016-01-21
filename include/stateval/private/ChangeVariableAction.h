#ifndef CHANGE_VARIABLE_ACTION_H
#define CHANGE_VARIABLE_ACTION_H

/* STD */
#include <string>

/* local */
#include "Action.h"
#include "stateval/Variable.h"

/* forwared declaration */
class StateMachine;

// does something
class ChangeVariableAction : public Action
{
public:
  ChangeVariableAction(const std::string &changeVar, Variable *var);
  ~ChangeVariableAction();

  void run() const;

private:
  Variable *mVar;
  std::string mChangeVar;
};

#endif // CHANGE_VARIABLE_ACTION_H
