#ifdef HAVE_CONFIG_H
  #include <config.h>
#endif

/* Project */
#include "stateval/Condition.h"
#include "stateval/GlobalVariables.h"
#include "stateval/Variable.h"

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
  // TODO: this below is only a test; implement function correct!
  AbstractVariable *val = global.getVariable ("tuner.available");
  
  return mVal->equals (val);
}

void  Condition::addComparison (const std::string &var, const AbstractVariable *val)
{
  mVar = var;
  mVal = val;
}
