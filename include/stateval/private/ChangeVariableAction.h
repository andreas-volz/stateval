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
  ChangeVariableAction(const std::string &view, const std::string &widget, const std::string &variable);
  ~ChangeVariableAction();

  void run() const;

private:
  std::string mView;
  std::string mWidget;
  std::string mVariable;
};

#endif // CHANGE_VARIABLE_ACTION_H
