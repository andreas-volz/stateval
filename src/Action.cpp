#ifdef HAVE_CONFIG_H
  #include <config.h>
#endif

/* Project */
#include "../include/stateval/Action.h"

/* STD */
#include <iostream>

using namespace std;

Action::Action (const std::string &command) :
  mCommand (command)
{
}

void Action::run () const
{
  cout << "Action: " << mCommand << endl;
}