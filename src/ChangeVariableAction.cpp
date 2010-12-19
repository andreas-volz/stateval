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
  // no need to free 'mVar' as it's only a pointer to a Loader managed variable...
}

void ChangeVariableAction::run () const
{
  GlobalVariables &global = GlobalVariables::instance ();
  global.changeVariable (mChangeVar, *mVar);
}
