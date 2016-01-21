#ifndef WIDGET_H
#define WIDGET_H

/* STD */
#include <string>

/* local */
#include "Logger.h"
#include "stateval/Variable.h"

class Widget
{
public:
  Widget(const std::string &name, const Variable *value);
  virtual ~Widget();

  const std::string getName() const;
  //const std::string getVariable() const;

  virtual void setValue(const Variable &val); // TODO: virtual?

  virtual void updateContent(); // TODO: virtual? // protected and friend to View?

protected:
  Logger mLogger;
  std::string mName;
  Variable *mValue;
};

#endif // WIDGET_H
