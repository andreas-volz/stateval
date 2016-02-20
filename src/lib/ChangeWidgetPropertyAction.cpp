#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

/* Project */
#include "stateval/private/ChangeWidgetPropertyAction.h"
#include "stateval/StateMachineAccessor.h"

/* STD */
#include <iostream>

using namespace std;

ChangeWidgetPropertyAction::ChangeWidgetPropertyAction(const std::string &view, const std::string &widget, const std::string &property, const std::string &variable) :
  mView(view),
  mWidget(widget),
  mProperty(property),
  mVariable(variable)
{
}

ChangeWidgetPropertyAction::~ChangeWidgetPropertyAction()
{
  // no need to free 'mVar' as it's only a pointer to a Loader managed variable...
}

void ChangeWidgetPropertyAction::run() const
{
  StateMachineAccessor &stateMachineAccessor = StateMachineAccessor::getInstance();
  Widget *widgetObj = stateMachineAccessor.getWidget(mView, mWidget);
  
  Variable *value = stateMachineAccessor.getVariable(mVariable);
  widgetObj->setProperty(mProperty, *value);
}
