#ifndef FIRE_EVENT_ACTION_H
#define FIRE_EVENT_ACTION_H

/* STD */
#include <string>

/* Project */
#include "Action.h"

/* forwared declaration */
class StateMachine;

// does something
class FireEventAction : public Action
{
public:
  FireEventAction (const std::string &event);

  void run () const;
    
private:
  const std::string mEvent;
};

#endif // FIRE_EVENT_ACTION_H
