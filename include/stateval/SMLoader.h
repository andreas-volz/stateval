#ifndef SM_LOADER_H
#define SM_LOADER_H

/* STD */
#include <string>
#include <vector>

/* Project */
#include "State.h"

class SMLoader
{
public:
  SMLoader ();
  virtual ~SMLoader ();
  
  bool load (const std::string &smDir);
  
  void unload ();
  
  void addEvent (const std::string &event);
  
  void addState (State *state);
  
  void addView (View *view);
  
  State *getInitialState ();
  
  int findMapingEvent (const std::string &event);
  
private:
  void fromEventsStream (std::ifstream &in_stream);
  void fromViewsStream (std::ifstream &in_stream, const std::string &smDir);
  void fromStatesStream (std::ifstream &in_stream);
  void fromTransitionsStream (std::ifstream &in_stream);
  
  std::vector <State*> mStateList;
  std::vector <View*> mViewList;
  std::map <std::string, int> mEventList;

  // this container is only used while building up the state machine structure
  // for later memory optimization of big state machines it could be destroyed
  // after construction...
  std::map <std::string, unsigned int> mStateNameMapper;

  // this container is only used while building up the state machine structure
  // for later memory optimization of big state machines it could be destroyed
  // after construction...
  std::map <std::string, unsigned int> mViewNameMapper;
  
  int eventCounter;
};

#endif // SM_LOADER_H
