#ifndef LOADER_H
#define LOADER_H

/* STD */
#include <string>
#include <vector>

/* Project */
#include "State.h"

/* pluxx */
#include <pluxx/Plugin.h>

class Loader : public pluxx::Plugin
{
public:
  Loader ();
  virtual ~Loader ();
  
  virtual bool load (const std::string &smDir) = 0;
  
  virtual void unload () = 0;
  
  void addEvent (const std::string &event);
  
  void addState (State *state);
  
  void addView (View *view);
  
  State *getInitialState ();
  
  int findMapingEvent (const std::string &event);

protected:
  std::vector <State*> mStateList;
  std::vector <View*> mViewList;
  std::map <std::string, int> mEventList;

private:
  int eventCounter;
};

#endif // LOADER_H

