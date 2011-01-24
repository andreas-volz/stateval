#ifndef ACTION_H
#define ACTION_H

#include <string>

class Action
{
public:

  virtual ~Action(){}
    
  virtual void run () const = 0;
    
private:
};

#endif // ACTION_H
