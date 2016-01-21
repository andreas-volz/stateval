#ifndef CONDITION_H
#define CONDITION_H

/* STD */
#include <list>

/* Project */
#include "State.h"
#include "stateval/Variable.h"

/* forward declaration */

class Condition
{
public:
  Condition();
  virtual ~Condition();

  bool evaluate() const;

  void addComparison(const std::string &var, const Variable *val);

private:
  std::string mVar;
  const Variable *mVal;
};

#endif // CONDITION_H
