#ifdef HAVE_CONFIG_H
  #include <config.h>
#endif

/* local */
#include "EdjeView.h"
#include "EdjeContext.h"

/* stateval */
#include "stateval/stateval.h"

/* STD */
#include <iostream>

using namespace std;

const int width = 800;
const int height = 600;

static const char* type = "View";
static const unsigned int major_version = 1;
static const unsigned int minor_version = 1;

EdjeView::EdjeView (Context *context, const std::list <std::string> &params) :
  mStateMachineAccess (&StateMachineAccess::instance ()),
  mEdjeContext (NULL),
  mEvas (NULL),
  mEdje (NULL),
  groupState (Unrealized),
  param ("label", "blub")
{
  assert (context);
  
  //if (params.length () != 2)
    //throw something
  
  std::list <std::string>::const_iterator params_it = params.begin ();
  mFilename = *params_it;
  ++params_it;
  mGroupname = *params_it;

  mEdjeContext = static_cast <EdjeContext*> (context);
  mEvas = &mEdjeContext->getCanvas ();
  
  mRealizeDispatcher.signalDispatch.connect (sigc::mem_fun (this, &EdjeView::realizeDispatched));
  mUnrealizeDispatcher.signalDispatch.connect (sigc::mem_fun (this, &EdjeView::unrealizeDispatched));
}

const std::string EdjeView::getType ()
{
  return type;
}

const unsigned int EdjeView::getMajorVersion ()
{
  return major_version;
}

const unsigned int EdjeView::getMinorVersion ()
{
  return minor_version;
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

  mEdje->resize (mEdjeContext->getResolution ());

  GlobalVariables &global = GlobalVariables::instance ();
  
  for (WidgetIterator wl_it = beginOfWidgets ();
       wl_it != endOfWidgets ();
       ++wl_it)
  {
    const Widget &w = *wl_it;
    
    Eflxx::CountedPtr <Edjexx::Part> part (mEdje->getPart ("Text_Example"));
    AbstractVariable *val = global.getVariable (w.getVariable ());
    assert (val);

    /*
     TODO:
     Get variable and put content based on it's type into widget.
     This needs much more work and a general design decision!
     */

    if (val->getType () == AbstractVariable::TYPE_STRING)
    {
      String *str = static_cast <String*> (val);
      Edjexx::ExternalParam param ("text", str->getData ());

      part->setParam (&param);
    }
    
    cout << "Widget name: " << w.getName () << endl;
    cout << "Widget variable: " << w.getVariable () << endl;
  }
  
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

/*****************************/
/* Plugin needed C functions */
/*****************************/

PLUGIN_EXPORT EdjeView *plugin_create (Context *context, const std::list <std::string> &params)
{
  return new EdjeView (context, params);
}

PLUGIN_EXPORT void plugin_destroy (View *plugin)
{
  delete plugin;
}

PLUGIN_EXPORT const char *get_plugin_type ()
{
  return type;
}

PLUGIN_EXPORT unsigned int get_plugin_major_version ()
{
  return major_version;
}
