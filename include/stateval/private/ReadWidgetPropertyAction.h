#ifndef READ_WIDGET_PROPERTY_ACTION_H
#define READ_WIDGET_PROPERTY_ACTION_H

/* STD */
#include <string>

/* local */
#include "Action.h"
#include "stateval/Variable.h"

/* forwared declaration */
class StateMachine;

class ReadWidgetPropertyAction : public Action
{
public:
  ReadWidgetPropertyAction(const std::string &view, const std::string &widget, const std::string &property, const std::string &variable);
  ~ReadWidgetPropertyAction();

  void run() const;

private:
  std::string mView;
  std::string mWidget;
  std::string mProperty;
  std::string mVariable;
};

#endif // READ_WIDGET_PROPERTY_ACTION_H
