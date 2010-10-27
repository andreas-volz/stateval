#ifndef LOADER_H
#define LOADER_H

/* STD */
#include <string>
#include <vector>

/* local */
#include "State.h"

/* pluxx */
#include <pluxx/Plugin.h>

/* forward delarations */
class Context;

class Loader : public pluxx::Plugin
{
public:
  Loader ();
  virtual ~Loader ();
  
  virtual bool load (Context *context, const std::string &smDir) = 0;
  
  virtual void unload () = 0;
  
  void addEvent (const std::string &event);
  
  void addState (State *state);

  void addAction (Action *action);
  
  void addView (View *view);
  
  State *getInitialState ();
  
  int findMapingEvent (const std::string &event);

protected:
  View *loadView (const std::string &viewPlugin, Context *context, const std::list <std::string> &params);
  
  std::vector <State*> mStateList;
  std::vector <View*> mViewList;
  std::map <std::string, int> mEventList;
  std::list <Action*> mActionList;

private:
  int eventCounter;
};

#endif // LOADER_H

