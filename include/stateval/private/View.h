#ifndef VIEW_H
#define VIEW_H

/* STD */
#include <string>
#include <map>
#include <list>

/* pluxx */
#include <pluxx/Plugin.h>

/* local */
#include "Logger.h"

// TODO: move to separate header file
class Widget
{
public:
  Widget (const std::string &name, const std::string &variable);
  virtual ~Widget () {};

  const std::string getName () const;
  const std::string getVariable () const;
  
private:
  Logger mLogger;
  std::string mName;
  std::string mVariable;
};

class View : public pluxx::Plugin
{
public:
  typedef std::list <Widget>::const_iterator WidgetIterator;
  
  View ();
  virtual ~View () {};

  virtual void realize () = 0;
  
  virtual void unrealize () = 0;

  virtual void pushEvent (int event);
  
  void addEventMapping (const int rawEvent, const int mappedEvent);
  
  void mapEvent (int &inOutEvent);

  void addWidget (const Widget &w);

  WidgetIterator beginOfWidgets () {return mWidgetVariableList.begin ();}
  WidgetIterator endOfWidgets ()   {return mWidgetVariableList.end ();}

private:
  Logger mLogger;
  std::map <int, int> mEventMap;
  std::list <Widget> mWidgetVariableList;
};

#endif // VIEW_H
