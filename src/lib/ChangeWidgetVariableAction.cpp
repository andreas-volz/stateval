#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

/* Project */
#include "stateval/private/ChangeWidgetVariableAction.h"
#include "stateval/StateMachineAccessor.h"

/* STD */
#include <iostream>

using namespace std;

ChangeWidgetVariableAction::ChangeWidgetVariableAction(const std::string &view, const std::string &widget, const std::string &variable) :
  mView(view),
  mWidget(widget),
  mVariable(variable)
{
}

ChangeWidgetVariableAction::~ChangeWidgetVariableAction()
{
  // no need to free 'mVar' as it's only a pointer to a Loader managed variable...
}

void ChangeWidgetVariableAction::run() const
{
  StateMachineAccessor &stateMachineAccessor = StateMachineAccessor::getInstance();
  Widget *widgetObj = stateMachineAccessor.getWidget(mView, mWidget);

  Variable *value = stateMachineAccessor.getVariable(mVariable);
  widgetObj->setValue(*value);
}
