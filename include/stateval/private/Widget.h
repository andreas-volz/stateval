#ifndef WIDGET_H
#define WIDGET_H

/* STD */
#include <string>

/* local */
#include "Logger.h"

class Widget
{
public:
  Widget (const std::string &name, const std::string &variable);
  virtual ~Widget () {};

  const std::string getName () const;
  const std::string getVariable () const;
  
private:
  Logger mLogger;
  std::string mName;
  std::string mVariable;
};

#endif // WIDGET_H
