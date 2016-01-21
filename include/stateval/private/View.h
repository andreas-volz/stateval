#ifndef VIEW_H
#define VIEW_H

/* STD */
#include <string>
#include <map>
#include <list>

/* pluxx */
#include <pluxx/Plugin.h>

/* local */
#include "stateval/Variable.h"
#include "Logger.h"
#include "Widget.h"

class View
{
public:
  typedef std::map <std::string, Widget*>::const_iterator WidgetIterator;

  View();
  virtual ~View();

  virtual void realize() = 0;

  virtual void unrealize() = 0;

  void setLayer(int layer);

  int getLayer();

  virtual void pushEvent(int event);

  void addEventMapping(const int rawEvent, const int mappedEvent);

  void mapEvent(int &inOutEvent);

  virtual void createWidget(const std::string &name, const Variable *value) = 0;

  WidgetIterator beginOfWidgets()
  {
    return mWidgetVariableMap.begin();
  }
  WidgetIterator endOfWidgets()
  {
    return mWidgetVariableMap.end();
  }

protected:
  std::map <std::string, Widget*> mWidgetVariableMap;
  
private:
  Logger mLogger;
  std::map <int, int> mEventMap;
  
  int mLayer;
};

#endif // VIEW_H
