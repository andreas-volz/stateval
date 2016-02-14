#ifndef WIDGET_H
#define WIDGET_H

/* STD */
#include <string>

/* local */
#include "stateval/Variable.h"

class Widget
{
public:
  Widget(const std::string &name, const Variable *value);
  virtual ~Widget();

  const std::string getName() const;

  /* returns a copy of the widget variable => needs to be deleted afterwards */  
  virtual Variable *getValue() = 0;

  virtual void setValue(const Variable &value);

  virtual void updateContent(); // TODO: virtual? // protected and friend to View?

protected:
  std::string mName;
  Variable *mValue;
};

#endif // WIDGET_H
