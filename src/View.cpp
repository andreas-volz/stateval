#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

/* Project */
#include "stateval/View.h"

/* STD */
#include <iostream>

using namespace std;

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
