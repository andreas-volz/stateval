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
  FireEventAction (int event);

  void run () const;
    
private:
  int mEvent;
};

#endif // FIRE_EVENT_ACTION_H
