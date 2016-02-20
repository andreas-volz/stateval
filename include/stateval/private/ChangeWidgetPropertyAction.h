#ifndef CHANGE_WIDGET_PROPERTY_ACTION_H
#define CHANGE_WIDGET_PROPERTY_ACTION_H

/* STD */
#include <string>

/* local */
#include "Action.h"
#include "stateval/Variable.h"

/* forwared declaration */
class StateMachine;

class ChangeWidgetPropertyAction : public Action
{
public:
  ChangeWidgetPropertyAction(const std::string &view, const std::string &widget, const std::string &property, const std::string &variable);
  ~ChangeWidgetPropertyAction();

  void run() const;

private:
  std::string mView;
  std::string mWidget;
  std::string mProperty;
  std::string mVariable;
};

#endif // CHANGE_WIDGET_PROPERTY_ACTION_H
