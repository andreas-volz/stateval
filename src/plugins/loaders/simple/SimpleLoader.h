#ifndef SIMPLE_LOADER_H
#define SIMPLE_LOADER_H

/* STD */
#include <string>
#include <vector>

/* stateval */
#include "stateval/State.h"
#include "stateval/Loader.h"

class SimpleLoader : public Loader
{
public:
  SimpleLoader ();
  virtual ~SimpleLoader ();

  const std::string getType ();
  
  const unsigned int getMajorVersion ();

  const unsigned int getMinorVersion ();
  
  bool load (Context *context, const std::string &smDir);
  
  void unload ();
  
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
  void fromViewsStream (Context *context, std::ifstream &in_stream, const std::string &smDir);

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

  std::map <std::string, unsigned int> mStateNameMapper;
  std::map <std::string, unsigned int> mViewNameMapper;
};

#endif // SIMPLE_LOADER_H
