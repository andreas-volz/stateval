#ifdef HAVE_CONFIG_H
  #include <config.h>
#endif

/* Project */
#include "../include/stateval/EdjeView.h"
#include "../include/stateval/StateMachineAccess.h"

/* STD */
#include <iostream>

const int width = 800;
const int height = 600;

using namespace std;

EdjeView::EdjeView (const std::string &filename, const std::string &groupname) :
  mStateMachineAccess (&StateMachineAccess::instance ()),
  mGraphicContext (&GraphicContext::instance ()),
  mEvas (&mGraphicContext->getCanvas ()),
  mEdje (NULL),
  mFilename (filename),
  mGroupname (groupname),
  groupState (Unrealized)
{
  mRealizeDispatcher.signalDispatch.connect (sigc::mem_fun (this, &EdjeView::realizeDispatched));
  mUnrealizeDispatcher.signalDispatch.connect (sigc::mem_fun (this, &EdjeView::unrealizeDispatched));
}

void EdjeView::realize ()
{  
  mRealizeDispatcher.signal ();
}

void EdjeView::unrealize ()
{
  cout << "+EdjeView::unrealize ()" << endl;
  
  mUnrealizeDispatcher.signal ();
  
  // wait for animation finished on statemachine thread
  condUnrealize.wait (mutexUnrealize);

  groupState = Unrealized;

  cout << "-EdjeView::unrealize ()" << endl;
}

void EdjeView::realizeDispatched (int missedEvents)
{
  cout << "EdjeView::realize ()" << endl;
  
  cout << "Filename: '" << mFilename << "', Groupname: " << mGroupname << endl;
      
  mEdje = new Edjexx::Object (*mEvas, mFilename, mGroupname);
  
  // connect visible/invisible handler
  mEdje->connect ("invisible_signal", "edje", sigc::mem_fun (this, &EdjeView::invisibleFunc));
  mEdje->connect ("visible_signal", "edje", sigc::mem_fun (this, &EdjeView::visibleFunc));

  mEdje->connect ("*", "edje", sigc::mem_fun (this, &EdjeView::edjeFunc));
  mEdje->connect ("*", "framework", sigc::mem_fun (this, &EdjeView::frameworkFunc));
  
  mEdje->connect ("*", "*", sigc::mem_fun (this, &EdjeView::allFunc));

  // TODO: think about resizing to group min/max if available
  mEdje->resize (mGraphicContext->getResolution ());

  mEdje->setLayer (0);
  mEdje->show ();

  groupState = Realizing;
  mEdje->emit ("visible", "framework");
}

void EdjeView::unrealizeDispatched (int missedEvents)
{
  if (mEdje)
  {
    groupState = Unrealizing;
    mEdje->emit ("invisible", "framework");
  }
}

void EdjeView::invisibleFunc (const std::string emmision, const std::string source)
{
  cout << "invisibleFunc" << endl;

  delete mEdje;
  mEdje = NULL;
  
  // signal the edje statemachine thread that the animation is finished
  condUnrealize.signal ();
}

void EdjeView::visibleFunc (const std::string emmision, const std::string source)
{
  cout << "visibleFunc" << endl;
  
  groupState = Realized;
}

void EdjeView::frameworkFunc (const std::string emmision, const std::string source)
{
  cout << "frameworkFunc: " << emmision << ", " << source << endl;
}

void EdjeView::edjeFunc (const std::string emmision, const std::string source)
{
  cout << "edjeFunc: " << emmision << ", " << source << endl;
}

void EdjeView::allFunc (const std::string emmision, const std::string source)
{
  cout << "allFunc: " << emmision << ", " << source << endl;
  string event (source + "#" + emmision);

  // only push new events for realized screens
  // when I do this it leads into freezes as the invisible signal doesn't come
  //if (groupState != Realized) return;
  
  if (mStateMachineAccess->findMapingEvent (event) != -1)
  {
    cout << "mStateMachineAccess->pushEvent" << endl;
    mStateMachineAccess->pushEvent (event); //"Button01#clicked"
  }
}
