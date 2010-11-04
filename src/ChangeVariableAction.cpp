#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

/* Project */
#include "stateval/ChangeVariableAction.h"
#include "stateval/StateMachineAccess.h"
#include "stateval/GlobalVariables.h"

/* STD */
#include <iostream>

using namespace std;

ChangeVariableAction::ChangeVariableAction (const std::string &changeVar, AbstractVariable *var) :
  mVar (var),
  mChangeVar (changeVar)
{
}

ChangeVariableAction::~ChangeVariableAction ()
{
  delete mVar;
}

void ChangeVariableAction::run () const
{
  GlobalVariables &global = GlobalVariables::instance ();
  global.changeVariable (mChangeVar, *mVar);
}
