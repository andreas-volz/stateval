#ifdef HAVE_CONFIG_H
  #include <config.h>
#endif

/* local */
#include "stateval/Condition.h"
#include "stateval/GlobalVariables.h"
#include "stateval/Variable.h"

/* STD */
#include <cassert>

Condition::Condition () :
  mVal (NULL)
{
}

Condition::~Condition ()
{
  delete mVal;
}

bool Condition::evaluate () const
{
  GlobalVariables &global = GlobalVariables::instance ();
  
  AbstractVariable *val = global.getVariable (mVar);
  assert (val);

  // TODO: not sure what happens if it compares two different variable types...
  
  return mVal->equals (val);
}

void  Condition::addComparison (const std::string &var, const AbstractVariable *val)
{
  mVar = var;
  mVal = val;
}
