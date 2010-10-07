#ifndef VIEW_H
#define VIEW_H

/* STD */
#include <string>
#include <map>

/* pluxx */
#include <pluxx/Plugin.h>

class View : public pluxx::Plugin
{
public:
  View ();

  virtual void realize () = 0;
  
  virtual void unrealize () = 0;
  
  void addEventMapping (const int rawEvent, const int mappedEvent);
  
  void mapEvent (int &inOutEvent);

private:
  std::map <int, int> mEventMap;
};

#endif // VIEW_H
