#ifdef HAVE_CONFIG_H
  #include <config.h>
#endif

/* local */
#include "EdjeView.h"
#include "EdjeContext.h"
#include "Logger.h"

/* Eflxx */
#include <elementaryxx/Elementaryxx.h>

/* stateval */
#include "stateval/stateval.h"
#include "stateval/private/stateval_private.h"

/* STD */
#include <iostream>

using namespace std;

const int width = 800;
const int height = 600;

static const char* type = "View";
static const unsigned int major_version = 1;
static const unsigned int minor_version = 1;

Logger logger ("stateval.plugins.views.edje");

EdjeView::EdjeView (Context *context, const std::list <std::string> &params) :
  mEdjeContext (NULL),
  mEvas (NULL),
  mEdje (NULL),
  groupState (Unrealized),
  param ("label", "blub")
{
  assert (context);
  
  if (params.size () != 2)
  {
    assert (false);
  }
  
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
  LOG4CXX_TRACE (logger, "+EdjeView::unrealize ()");
  
  mUnrealizeDispatcher.signal ();
  
  // wait for animation finished on statemachine thread
  condUnrealize.wait (mutexUnrealize);

  groupState = Unrealized;

  LOG4CXX_TRACE (logger, "-EdjeView::unrealize ()");
}

void EdjeView::realizeDispatched (int missedEvents)
{
  LOG4CXX_TRACE (logger, "EdjeView::realize ()");
  
  LOG4CXX_INFO (logger, "Filename: '" << mFilename << "', Groupname: " << mGroupname);
      
  mEdje = new Edjexx::Object (*mEvas, mFilename, mGroupname);
  
  // connect visible/invisible handler
  mEdje->connect ("invisible_signal", "edje", sigc::mem_fun (this, &EdjeView::invisibleFunc));
  mEdje->connect ("visible_signal", "edje", sigc::mem_fun (this, &EdjeView::visibleFunc));

  mEdje->connect ("*", "edje", sigc::mem_fun (this, &EdjeView::edjeFunc));
  mEdje->connect ("*", "stateval", sigc::mem_fun (this, &EdjeView::statevalFunc));

  mEdje->connect ("*", "*", sigc::mem_fun (this, &EdjeView::allFunc));

  mEdje->resize (mEdjeContext->getResolution ());

  updateContent ();

  mEdje->setLayer (0);
  mEdje->show ();

  groupState = Realizing;
  mEdje->emit ("visible", "stateval");
}

void EdjeView::unrealizeDispatched (int missedEvents)
{
  if (mEdje)
  {
    groupState = Unrealizing;
    mEdje->emit ("invisible", "stateval");
  }
}

void EdjeView::updateContent ()
{ 
  // FIXME: seems first screen could not do a updateContent ()!!
  if (!mEdje)
    return;
  
  GlobalVariables &global = GlobalVariables::instance ();
  
  for (WidgetIterator wl_it = beginOfWidgets ();
       wl_it != endOfWidgets ();
       ++wl_it)
  {
    const Widget &w = *wl_it;
    
    AbstractVariable *val = global.getVariable (w.getVariable ());
    assert (val);

    try
    {
      Edjexx::Part &part = mEdje->getPart (w.getName ());

      if (val->getType () == AbstractVariable::TYPE_STRUCT)
      {
        Struct *st = static_cast <Struct*> (val);
        bool specialHandled = false;

        // TODO: is there a special handling for struct types needed?
        try 
        {
          Evasxx::Object &ext_eo3 = part.getExternalObject ();
          Evasxx::Object &eo3 = part.getSwallow ();
          LOG4CXX_DEBUG (logger, "Edje External Widget type: " << ext_eo3.getType ());
          LOG4CXX_DEBUG (logger, "Edje Part Widget type: " << eo3.getType ());
          
          if (ext_eo3.getType () == "elm_widget")
          {
            Elmxx::Object &elm_object = *(static_cast <Elmxx::Object*> (&ext_eo3));

            LOG4CXX_DEBUG (logger, "Elm Widget type: " << elm_object.getWidgetType ());
            // TODO: slider is now generic supported. But ElmList needs to be implemented...
            /*if (elm_object.getWidgetType () == "slider")
            {
              Elmxx::Slider &slider = *(static_cast <Elmxx::Slider*> (&elm_object));
              AbstractVariable *av1 = st->getData ("label");
              if (av1->getType () == AbstractVariable::TYPE_STRING)
              {
                String *s1 = static_cast <String*> (av1);
                slider.setLabel (s1->getData ());
              }
              
              AbstractVariable *av2 = st->getData ("value");
              if (av2->getType () == AbstractVariable::TYPE_FLOAT)
              {
                Float *f1 = static_cast <Float*> (av2);
                slider.setValue (f1->getData ());
              }
              specialHandled = true;
            }*/
          }
        }
        catch (Edjexx::ExternalNotExistingException ene)
        {
          cerr << ene.what () << endl;
        }

        // generic widget type handling
        if (!specialHandled)
        {
          for (Struct::Iterator s_it = st->begin ();
               s_it != st->end ();
               ++s_it)
          {
            const string &name = s_it->first;
            AbstractVariable *av = s_it->second;

            if (av)
            {
              if (av->getType () == AbstractVariable::TYPE_STRING)
              {
                String *str = static_cast <String*> (av);
                Edjexx::ExternalParam param (name, str->getData ());
                part.setParam (&param);
              }
              else if (av->getType () == AbstractVariable::TYPE_FLOAT)
              {
                Float *f = static_cast <Float*> (av);
                Edjexx::ExternalParam param (name, f->getData ());
                part.setParam (&param);
              }
              else if (av->getType () == AbstractVariable::TYPE_BOOL)
              {
                Bool *b = static_cast <Bool*> (av);
                Edjexx::ExternalParam param (name, b->getData ());
                part.setParam (&param);
              }
            }
          }
        }
      }
      else if (val->getType () == AbstractVariable::TYPE_LIST)
      {
        try 
        {
          List *ls = static_cast <List*> (val);
          
          Evasxx::Object &ext_eo3 = part.getExternalObject ();
          Evasxx::Object &eo3 = part.getSwallow ();
          LOG4CXX_DEBUG (logger, "Edje External Widget type: " << ext_eo3.getType ());
          LOG4CXX_DEBUG (logger, "Edje Part Widget type: " << eo3.getType ());
          
          if (ext_eo3.getType () == "elm_widget")
          {
            Elmxx::Object &elm_object = *(static_cast <Elmxx::Object*> (&ext_eo3));

            LOG4CXX_DEBUG (logger, "Elm Widget type: " << elm_object.getWidgetType ());

            if (elm_object.getWidgetType () == "list")
            {
              Elmxx::List &list = *(static_cast <Elmxx::List*> (&elm_object));

              // TODO: I think until the edited/merge feature is implemented it's the
              // best to clear the list before adding new elements...
              list.clear ();
              for (List::Iterator ls_it = ls->begin ();
                   ls_it != ls->end ();
                   ++ls_it)
              {
                AbstractVariable *av = *ls_it;

                if (av->getType () == AbstractVariable::TYPE_STRING)
                {
                  String *str = static_cast <String*> (av);
                  list.append (str->getData (), NULL, NULL);
                }
                list.go ();
              }
            }                
          }
        }
        catch (Edjexx::ExternalNotExistingException ene)
        {
          cerr << ene.what () << endl;
        }
      }
      else if (val->getType () == AbstractVariable::TYPE_STRING)
      {
        String *str = static_cast <String*> (val);
        
        part.setText (str->getData ());
      }
      else
      {
        LOG4CXX_WARN (logger, "Currently not supported AbstractVariable Type!");
      }
    }
    catch (Edjexx::PartNotExistingException pne)
    {
      cerr << pne.what () << endl;
    }
    
    LOG4CXX_INFO (logger, "Widget name: " << w.getName ());
    LOG4CXX_INFO (logger, "Widget variable: " << w.getVariable ());
  }
}

void EdjeView::invisibleFunc (const std::string emmision, const std::string source)
{
  LOG4CXX_TRACE (logger, "invisibleFunc");

  groupState = Unrealized;
  delete mEdje;
  mEdje = NULL;
  
  // signal the edje statemachine thread that the animation is finished
  condUnrealize.signal ();
}

void EdjeView::visibleFunc (const std::string emmision, const std::string source)
{
  LOG4CXX_TRACE (logger, "visibleFunc");
  
  groupState = Realized;
}

void EdjeView::statevalFunc (const std::string emmision, const std::string source)
{
  LOG4CXX_TRACE (logger, "statevalFunc: " << emmision << ", " << source);
}

void EdjeView::edjeFunc (const std::string emmision, const std::string source)
{
  LOG4CXX_TRACE (logger, "edjeFunc: " << emmision << ", " << source);
}

void EdjeView::allFunc (const std::string emmision, const std::string source)
{
  if (source != "stateval")
  {
    StateMachineAccess &stateMachineAccess (StateMachineAccess::instance ());
    
    LOG4CXX_DEBUG (logger, "allFunc: " << emmision << ", " << source);
    string event ("edje," + source + "," + emmision);

    // only push new events for realized screens
    // when I do this it leads into freezes as the invisible signal doesn't come
    //if (groupState != Realized) return;
    
    if (stateMachineAccess.findMapingEvent (event) != -1)
    {
      LOG4CXX_DEBUG (logger, "mStateMachineAccess->pushEvent");
      stateMachineAccess.pushEvent (event);
    }
  }
}

void EdjeView::pushEvent (int event)
{
  if (groupState == Realized)
  {
    StateMachineAccess &stateMachineAccess (StateMachineAccess::instance ());
    
    static const int VIEW_UPDATE_EVENT = stateMachineAccess.findMapingEvent ("VIEW_UPDATE");
    
    string eventString = stateMachineAccess.findMapingEvent (event);

    LOG4CXX_DEBUG (logger, "EdjeView::smEvents: " << event << " / " << eventString);

    if (eventString.substr (4) != "edje")
    {
      mEdje->emit (eventString, "stateval");
    }

    if (event == VIEW_UPDATE_EVENT)
    {
      updateContent ();
    }
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
