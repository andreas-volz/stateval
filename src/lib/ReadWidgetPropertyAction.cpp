#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

/* Project */
#include "stateval/private/ReadWidgetPropertyAction.h"
#include "stateval/StateMachineAccessor.h"

/* STD */
#include <iostream>

using namespace std;

ReadWidgetPropertyAction::ReadWidgetPropertyAction(const std::string &view, const std::string &widget, const std::string &property, const std::string &variable) :
  mView(view),
  mWidget(widget),
  mProperty(property),
  mVariable(variable)
{
}

ReadWidgetPropertyAction::~ReadWidgetPropertyAction()
{
  // no need to free 'mVar' as it's only a pointer to a Loader managed variable...
}

void ReadWidgetPropertyAction::run() const
{
  StateMachineAccessor &stateMachineAccessor = StateMachineAccessor::getInstance();
  Widget *widgetObj = stateMachineAccessor.getWidget(mView, mWidget);
  
  Variable *value = widgetObj->getProperty(mProperty);

  if(value)
  {
    stateMachineAccessor.changeVariable(mVariable, *value);
    delete value;
  }
}
