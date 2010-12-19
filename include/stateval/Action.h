#ifndef ACTION_H
#define ACTION_H

#include <string>

// does something
class Action
{
public:
  //Action () {};
  //Action (const std::string &command);

  virtual ~Action(){}
    
  virtual void run () const = 0;
    
private:
  //std::string mCommand;
};

#endif // ACTION_H
