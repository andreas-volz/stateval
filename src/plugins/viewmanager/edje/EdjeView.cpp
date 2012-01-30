#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

/* local */
#include "EdjeView.h"
#include "EdjeContext.h"

/* Eflxx */
#include <elementaryxx/Elementaryxx.h>

/* stateval */
#include "stateval/stateval.h"
#include "stateval/private/stateval_private.h"

/* STD */
#include <iostream>

using namespace std;

EdjeView::EdjeView(EdjeContext *context, const std::map <std::string, std::string> &params) :
  mLogger("stateval.plugins.views.edje"),
  mEdjeContext(context),
  groupState(Unrealized),
  mEvent (-1)
{
  assert(context);
  
  std::map <std::string, std::string>::const_iterator param_it;

  param_it = params.find ("filename");
  if (param_it != params.end ())
  {
    mFilename = param_it->second;
  }
  else
  {
    // TODO: needed error handling
    assert (false);      
  }

  param_it = params.find ("groupname");
  if (param_it != params.end ())
  {
    mGroupname = param_it->second;
  }
  else
  {
    // TODO: needed error handling
    assert (false);      
  }

  LOG4CXX_TRACE(mLogger, "constructor: " << mFilename << "," << mGroupname);

//mEdjeContext = static_cast <EdjeContext *>(context);

  mRealizeDispatcher.signalDispatch.connect(sigc::mem_fun(this, &EdjeView::realizeDispatched));
  mUnrealizeDispatcher.signalDispatch.connect(sigc::mem_fun(this, &EdjeView::unrealizeDispatched));

  mPushEventDispatcher.signalDispatch.connect(sigc::mem_fun(this, &EdjeView::pushEventDispatched));
}

void EdjeView::realize()
{
  LOG4CXX_DEBUG(mLogger, "+wait for realize");
  mRealizeDispatcher.emit();

  mutexRealize.lock();
  condRealize.wait(mutexRealize);
  mutexRealize.unlock();
  LOG4CXX_DEBUG(mLogger, "-wait for realize");
}

void EdjeView::unrealize()
{
  LOG4CXX_TRACE(mLogger, "+unrealize ()");

  mUnrealizeDispatcher.emit();

  // wait for animation finished on statemachine thread
  mutexUnrealize.lock();
  condUnrealize.wait(mutexUnrealize);
  mutexUnrealize.unlock();

  groupState = Unrealized;

  LOG4CXX_TRACE(mLogger, "-unrealize ()");
}

void EdjeView::realizeDispatched(int missedEvents)
{
  LOG4CXX_TRACE(mLogger, "+realizeDispatched()");

  LOG4CXX_INFO(mLogger, "Filename: '" << mFilename << "', Groupname: " << mGroupname);

  mWindow = mEdjeContext->window;
  mLayout = Elmxx::Layout::factory(*mWindow);
  mLayout->setWeightHintSize(EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
  mWindow->addObjectResize(*mLayout);
  mLayout->show();

  mLayout->setFile(mFilename, mGroupname);

  LOG4CXX_INFO(mLogger, "Layer: " << getLayer());
  mLayout->setLayer(getLayer());

  Eflxx::CountedPtr <Edjexx::Object> edjeObj(mLayout->getEdje());

  // connect visible/invisible handler
  // --> TODO: while changing names connect both -> remove later deprecated names!
  edjeObj->connect("invisible_signal", "edje", sigc::mem_fun(this, &EdjeView::invisibleFunc));
  edjeObj->connect("visible_signal", "edje", sigc::mem_fun(this, &EdjeView::visibleFunc));
  //// <--

  // this is the new name of the spec!
  edjeObj->connect("animation,end", "invisible", sigc::mem_fun(this, &EdjeView::invisibleFunc));
  edjeObj->connect("animation,end", "visible", sigc::mem_fun(this, &EdjeView::visibleFunc));

  edjeObj->connect("*", "edje", sigc::mem_fun(this, &EdjeView::edjeFunc));
  edjeObj->connect("*", "stateval", sigc::mem_fun(this, &EdjeView::statevalFunc));

  edjeObj->connect("*", "*", sigc::mem_fun(this, &EdjeView::allFunc));

  mLayout->resize(mEdjeContext->resolution);

  updateContent();

  //mLayout->setLayer (0);

  groupState = Realizing;
  edjeObj->emit("visible", "stateval");

  condRealize.signal();

  LOG4CXX_TRACE(mLogger, "-realizeDispatched()");
}

void EdjeView::unrealizeDispatched(int missedEvents)
{
  if (mLayout)
  {
    groupState = Unrealizing;
    Eflxx::CountedPtr <Edjexx::Object> edjeObj = mLayout->getEdje();
    edjeObj->emit("invisible", "stateval");
  }
}

void EdjeView::updateContent()
{

  // FIXME: seems first screen could not do a updateContent ()!!
  if (!mLayout)
    return;

  StateMachineAccessor &stateMachineAccessor = StateMachineAccessor::getInstance();
  Eflxx::CountedPtr <Edjexx::Object> edjeObj(mLayout->getEdje());

  /* FIXME: this logic below is not correct because if a variable is connected to two different widgets it
            may not be updated correct. TODO: check this!
   */
  for (WidgetIterator wl_it = beginOfWidgets();
       wl_it != endOfWidgets();
       ++wl_it)
  {
    const Widget &w = *wl_it;

    AbstractVariable *val = stateMachineAccessor.getVariable(w.getVariable());
    assert(val);

    // update widget data only if update is needed
    if (val->needsUpdate ())
    {
      try
      {
        Edjexx::Part &part = edjeObj->getPart(w.getName());

        if (val->getType() == AbstractVariable::TYPE_STRUCT)
        {
          Struct *st = static_cast <Struct *>(val);
          bool specialHandled = false;

          // TODO: is there a special handling for struct types needed?
          try
          {
            Evasxx::Object &ext_eo3 = part.getExternalObject();
            Evasxx::Object &eo3 = part.getSwallow();
            LOG4CXX_DEBUG(mLogger, "Edje External Widget type: " << ext_eo3.getType());
            LOG4CXX_DEBUG(mLogger, "Edje Part Widget type: " << eo3.getType());

            if (ext_eo3.getType() == "elm_widget")
            {
              Elmxx::Object &elm_object = *(static_cast <Elmxx::Object *>(&ext_eo3));

              LOG4CXX_DEBUG(mLogger, "Elm Widget type: " << elm_object.getWidgetType());
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
            cerr << ene.what() << endl;
          }

          // generic widget type handling
          if (!specialHandled)
          {
            for (Struct::Iterator s_it = st->begin();
                 s_it != st->end();
                 ++s_it)
            {
              const string &name = s_it->first;
              AbstractVariable *av = s_it->second;

              if (av)
              {
                if (av->getType() == AbstractVariable::TYPE_STRING)
                {
                  String *str = static_cast <String *>(av);
                  Edjexx::ExternalParam param(name, str->getData());
                  part.setParam(&param);
                }
                else if (av->getType() == AbstractVariable::TYPE_FLOAT)
                {
                  Float *f = static_cast <Float *>(av);
                  Edjexx::ExternalParam param(name, f->getData());
                  part.setParam(&param);
                }
                else if (av->getType() == AbstractVariable::TYPE_BOOL)
                {
                  Bool *b = static_cast <Bool *>(av);
                  Edjexx::ExternalParam param(name, b->getData());
                  part.setParam(&param);
                }
              }
            }
          }
        }
        else if (val->getType() == AbstractVariable::TYPE_LIST)
        {
          try
          {
            List *ls = static_cast <List *>(val);

            Evasxx::Object &ext_eo3 = part.getExternalObject();
            Evasxx::Object &eo3 = part.getSwallow();
            LOG4CXX_DEBUG(mLogger, "Edje External Widget type: " << ext_eo3.getType());
            LOG4CXX_DEBUG(mLogger, "Edje Part Widget type: " << eo3.getType());

            if (ext_eo3.getType() == "elm_widget")
            {
              Elmxx::Object &elm_object = *(static_cast <Elmxx::Object *>(&ext_eo3));

              LOG4CXX_DEBUG(mLogger, "Elm Widget type: " << elm_object.getWidgetType());

              if (elm_object.getWidgetType() == "list")
              {
                Elmxx::List &list = *(static_cast <Elmxx::List *>(&elm_object));

                // TODO: I think until the edited/merge feature is implemented it's the
                // best to clear the list before adding new elements...
                list.clear();
                for (List::Iterator ls_it = ls->begin();
                     ls_it != ls->end();
                     ++ls_it)
                {
                  AbstractVariable *av = *ls_it;

                  if (av->getType() == AbstractVariable::TYPE_STRING)
                  {
                    String *str = static_cast <String *>(av);
                    list.append(str->getData(), NULL, NULL);
                  }
                  list.go();
                }
              }
            }
          }
          catch (Edjexx::ExternalNotExistingException ene)
          {
            cerr << ene.what() << endl;
          }
        }
        else if (val->getType() == AbstractVariable::TYPE_STRING)
        {
          String *str = static_cast <String *>(val);

          part.setText(str->getData());
        }
        else
        {
          LOG4CXX_WARN(mLogger, "Currently not supported AbstractVariable Type!");
        }
      }
      catch (Edjexx::PartNotExistingException pne)
      {
        cerr << pne.what() << endl;
      }

      // widget is updated, so reset need for update
      val->setUpdateFlag (false);

      LOG4CXX_INFO(mLogger, "Widget name: " << w.getName());
      LOG4CXX_INFO(mLogger, "Widget variable: " << w.getVariable());
    }
    
  }
}

void EdjeView::invisibleFunc(const std::string emmision, const std::string source)
{
  LOG4CXX_TRACE(mLogger, "invisibleFunc");

  groupState = Unrealized;
  mWindow->delObjectResize(*mLayout);
  mLayout->destroy();
  mLayout = NULL;
  //elm_object_part_content_unset 	(mLayout->obj (), mGroupname.c_str ());

  // signal the edje statemachine thread that the animation is finished
  condUnrealize.signal();
}

void EdjeView::visibleFunc(const std::string emmision, const std::string source)
{
  LOG4CXX_TRACE(mLogger, "visibleFunc");

  groupState = Realized;
}

void EdjeView::statevalFunc(const std::string emmision, const std::string source)
{
  LOG4CXX_TRACE(mLogger, "statevalFunc: " << emmision << ", " << source);
}

void EdjeView::edjeFunc(const std::string emmision, const std::string source)
{
  LOG4CXX_TRACE(mLogger, "edjeFunc: " << emmision << ", " << source);
}

void EdjeView::allFunc(const std::string emmision, const std::string source)
{
  if (source != "stateval")
  {
    StateMachineAccessor &StateMachineAccessor(StateMachineAccessor::getInstance());

    string event("edje," + source + "," + emmision);
    LOG4CXX_DEBUG(mLogger, "allFunc: " << event << " (" << mGroupname << ")");

    // only push new events for realized screens
    // when I do this it leads into freezes as the invisible signal doesn't come
    //if (groupState != Realized) return;

    if (StateMachineAccessor.findMapingEvent(event) != -1)
    {
      LOG4CXX_DEBUG(mLogger, "mStateMachineAccessor->pushEvent");
      StateMachineAccessor.pushEvent(event);
    }
  }
}

void EdjeView::pushEventDispatched(int missedEvents)
{
  if (groupState == Realized)
  {
    StateMachineAccessor &StateMachineAccessor(StateMachineAccessor::getInstance());

    static const int VIEW_UPDATE_EVENT = StateMachineAccessor.findMapingEvent("VIEW_UPDATE");

    string eventString = StateMachineAccessor.findMapingEvent(mEvent);

    LOG4CXX_DEBUG(mLogger, "EdjeView::smEvents: " << mEvent << " / " << eventString);

    if ((eventString.length() >= 4) && (eventString.substr(4) != "edje"))
    {
      Eflxx::CountedPtr <Edjexx::Object> edjeObj = mLayout->getEdje();
      edjeObj->emit(eventString, "stateval");
    }

    if (mEvent == VIEW_UPDATE_EVENT)
    {
      updateContent();
    }
  }

  mCondPushEvent.signal ();
}

// TODO: synchronize this method!!
void EdjeView::pushEvent(int event)
{
  mEvent = event;
  
  mPushEventDispatcher.emit ();

  mMutexPushEvent.lock();  
  mCondPushEvent.wait(mMutexPushEvent);
  mMutexPushEvent.unlock();
}
