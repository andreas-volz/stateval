#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

/* local */
#include "stateval/private/View.h"
#include "MemoryUtil.h"

/* STD */
#include <iostream>

using namespace std;

View::View() :
  mLogger("stateval.View")
{
}

View::~View()
{
  //delete_stl_container<std::map<std::string, Widget*> >(mWidgetVariableMap);
  // TODO: delete mWidgetVariableMap
};

void View::addEventMapping(const int rawEvent, const int mappedEvent)
{
  mEventMap[rawEvent] = mappedEvent;
}

void View::mapEvent(int &inOutEvent)
{
  map <int, int>::iterator iter = mEventMap.find(inOutEvent);
  if (iter != mEventMap.end())
  {
    int &mapEvent = iter->second;
    LOG4CXX_DEBUG(mLogger, "map event: " << iter->first << " : " << iter->second);
    inOutEvent = mapEvent;
  }
}

/*! this is implemented empty with the intention that child classes
 *  don't need any special event handling
 * e.g. some EdjeView needs it push events to the Edje Main loop
 */
void View::pushEvent(int event)
{
}

void View::setLayer(int layer)
{
  mLayer = layer;
}

int View::getLayer()
{
  return mLayer;
}

Widget *View::getWidget(const std::string &name)
{
  return mWidgetMap[name];
}
