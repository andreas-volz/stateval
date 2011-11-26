#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

/* local */
#include "stateval/private/Condition.h"
#include "stateval/StateMachineAccessor.h"
#include "stateval/Variable.h"

/* STD */
#include <cassert>

Condition::Condition() :
  mVal(NULL)
{
}

Condition::~Condition()
{
  // no need to free 'mVal' as it's only a pointer to a Loader managed variable...
}

bool Condition::evaluate() const
{
  StateMachineAccessor &stateMachineAccessor = StateMachineAccessor::getInstance();

  AbstractVariable *val = stateMachineAccessor.getVariable(mVar);
  assert(val);

  // TODO: not sure what happens if it compares two different variable types...

  return mVal->equals(val);
}

void  Condition::addComparison(const std::string &var, const AbstractVariable *val)
{
  mVar = var;
  mVal = val;
}
