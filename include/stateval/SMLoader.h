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
  /*!
   * Format description:
   *
   * file = states
   * format = event1
   *          event2
   *          ...
   *
   */
  void fromEventsStream (std::ifstream &in_stream);

  /*!
   * Format description:
   *
   * file = states
   * format = view:view_name:view_type:view_reference
   *          map:from_event->to_event
   *          map:from_event2->to_event2
   *          ...
   *
   * the last parameter depends on view_type
   */
  void fromViewsStream (std::ifstream &in_stream, const std::string &smDir);

  /*!
   * Format description:
   *
   * file = states
   * format = state_name:state_type:parent_name[:history|view]
   *
   * the last parameter depends on state_type
   */
  void fromStatesStream (std::ifstream &in_stream);

  /*!
   * Format description:
   *
   * file = transitions
   * format = from_state_name:[event:]->to_state_name
   *
   * if no event is given than it's a default transition
   */
  void fromTransitionsStream (std::ifstream &in_stream);
  
  std::vector <State*> mStateList;
  std::vector <View*> mViewList;
  std::map <std::string, int> mEventList;

  std::map <std::string, unsigned int> mStateNameMapper;
  std::map <std::string, unsigned int> mViewNameMapper;
  
  int eventCounter;
};

#endif // SM_LOADER_H
