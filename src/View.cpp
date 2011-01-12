#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

/* Project */
#include "stateval/View.h"

/* STD */
#include <iostream>

using namespace std;

Widget::Widget (const std::string &name, const std::string &variable) :
  mName (name),
  mVariable (variable)
{
}

const std::string Widget::getName () const
{
  return mName;
}

const std::string Widget::getVariable () const
{
  return mVariable;
}

View::View ()
{
}

void View::addEventMapping (const int rawEvent, const int mappedEvent)
{
  mEventMap[rawEvent] = mappedEvent;
}

void View::mapEvent (int &inOutEvent)
{
  map <int,int>::iterator iter = mEventMap.find (inOutEvent);
  if (iter != mEventMap.end ())
  {
    int &mapEvent = iter->second;
    cout << "map event: " << iter->first << " : " << iter->second << endl;
    inOutEvent = mapEvent;
  } 
}

void View::addWidget (const Widget &w)
{ 
  mWidgetVariableList.push_back (w);
}

void View::pushEvent (int event)
{
  // this is implemented empty with the intention that child classes
  // don't need any special event handling
  // e.g. some EdjeView needs it push events to the Edje Main loop
}

