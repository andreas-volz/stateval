#ifdef HAVE_CONFIG_H
  #include <config.h>
#endif

/* Project */
#include "../include/stateval/Condition.h"
#include "../include/stateval/GlobalVariables.h"
#include "../include/stateval/Variable.h"

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
  AbstractVariable *val = global.getVariable ("tuner.available");
  
  return mVal->compare (val);
}

void  Condition::addComparison (const std::string &var, const AbstractVariable *val)
{
  mVar = var;
  mVal = val;
}
