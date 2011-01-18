#ifndef VIEW_H
#define VIEW_H

/* STD */
#include <string>
#include <map>
#include <list>

/* pluxx */
#include <pluxx/Plugin.h>

class Widget
{
public:
  Widget (const std::string &name, const std::string &variable);

  const std::string getName () const;
  const std::string getVariable () const;
  
private:
  std::string mName;
  std::string mVariable;
};

class View : public pluxx::Plugin
{
public:
  typedef std::list <Widget>::const_iterator WidgetIterator;
  
  View ();

  virtual void realize () = 0;
  
  virtual void unrealize () = 0;

  virtual void pushEvent (int event);
  
  void addEventMapping (const int rawEvent, const int mappedEvent);
  
  void mapEvent (int &inOutEvent);

  void addWidget (const Widget &w);

  WidgetIterator beginOfWidgets () {return mWidgetVariableList.begin ();}
  WidgetIterator endOfWidgets ()   {return mWidgetVariableList.end ();}

private:
  std::map <int, int> mEventMap;
  std::list <Widget> mWidgetVariableList;
};

#endif // VIEW_H
