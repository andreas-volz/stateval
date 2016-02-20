#ifndef WIDGET_H
#define WIDGET_H

/* STD */
#include <string>

/* local */
#include "stateval/Variable.h"

class Widget
{
public:
  Widget(const std::string &name);
  virtual ~Widget();

  const std::string getName() const;

  /* returns a copy of the widget variable => needs to be deleted afterwards */  
  virtual Variable *getProperty(const std::string &name) = 0;

  virtual void setProperty(const std::string &name, const Variable &property);

  virtual void updateContent(); // TODO: virtual? // protected and friend to View?

protected:
  std::string mName;
  std::map<std::string, Variable*> mProperties;
};

#endif // WIDGET_H
