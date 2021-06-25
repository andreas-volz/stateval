#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

/* local */
#include "EdjeView.h"
#include "EdjeContext.h"

/* Eflxx */
//#include <elementaryxx/Elementaryxx.h>

/* stateval */
#include "stateval/stateval.h"
#include "stateval/private/stateval_private.h"
#include "EdjeWidget.h"

/* STD */
#include <iostream>

using namespace std;
using efl::eo::instantiate;

EdjeView::EdjeView(EdjeContext *context, const std::string &dir, const std::map <std::string, std::string> &params) :
  mLogger("stateval.plugins.viewmanager.edje.EdjeView"),
  mLayout(NULL),
  mEdjeContext(context),
  groupState(Unrealized),
  mEvent (-1)
{
  assert(context);
  
  std::map <std::string, std::string>::const_iterator param_it;

  param_it = params.find ("filename");
  if (param_it != params.end ())
  {
    if(dir.empty())
    {
      mFilename = param_it->second;
    }
    else
    {
      mFilename = dir + "/" + param_it->second;
    }    
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

  mRealizeDispatcher.signalDispatch.connect(sigc::mem_fun(this, &EdjeView::realizeDispatched));
  mUnrealizeDispatcher.signalDispatch.connect(sigc::mem_fun(this, &EdjeView::unrealizeDispatched));
  mPushEventDispatcher.signalDispatch.connect(sigc::mem_fun(this, &EdjeView::pushEventDispatched));
  mUpdateDispatcher.signalDispatch.connect(sigc::mem_fun(this, &EdjeView::updateDispatched));
}

void EdjeView::realize()
{
  LOG4CXX_DEBUG(mLogger, "+wait for realize");
  mRealizeDispatcher.emit();

  mMutexRealize.lock();
  mCondRealize.wait(mMutexRealize);
  mMutexRealize.unlock();
  LOG4CXX_DEBUG(mLogger, "-wait for realize");
}

void EdjeView::unrealize()
{
  LOG4CXX_TRACE(mLogger, "+unrealize ()");

  mUnrealizeDispatcher.emit();

  // wait for animation finished on statemachine thread
  /* FIXME: sometimes the lib hangs in this condition. 
     It seems invisibleFunc() isn't called or in the wrong moment. Need to trace this!!
     I only see this error if the application ends - many tries needed to reproduce */ 
  mMutexUnrealize.lock();
  mCondUnrealize.wait(mMutexUnrealize);
  mMutexUnrealize.unlock();

  groupState = Unrealized;

  LOG4CXX_TRACE(mLogger, "-unrealize ()");
}

void EdjeView::allFunc2(efl::layout::Signal s, efl::eina::basic_string_view<char> emission, efl::eina::basic_string_view<char>  source)
{
  cout << "allFunc2(): " << emission << " / " << source << endl;

 
}

void EdjeView::realizeDispatched(int missedEvents)
{
  LOG4CXX_TRACE(mLogger, "+realizeDispatched()");

  LOG4CXX_INFO(mLogger, "Filename: '" << mFilename << "', Groupname: " << mGroupname);

  mWindow = mEdjeContext->window;
  mLayout = new efl::ui::Layout(instantiate, *mWindow);
  
  mLayout->hint_weight_set(EFL_GFX_HINT_EXPAND, EFL_GFX_HINT_EXPAND);
  mWindow->content_set(*mLayout);
  //mLayout->show();
  
  mLayout->file_set(mFilename);
  mLayout->key_set(mGroupname);

  LOG4CXX_INFO(mLogger, "Layer: " << getLayer());
  mLayout->layer_set(getLayer());

  mLayout->load();

  //Eflxx::CountedPtr <Edjexx::Object> edjeObj(mLayout->getEdje());

  // connect visible/invisible handler
  // --> TODO: while changing names connect both -> remove later deprecated names!
   mLayout->signal_callback_add("invisible_signal", "edje", [this] (auto&& signal, auto&&
    emission, auto&& source) {
    this->invisibleFunc(signal, emission.c_str(), source.c_str()); 
  });

  mLayout->signal_callback_add("visible_signal", "edje", [this] (auto&& signal, auto&&
    emission, auto&& source) {
    this->visibleFunc(signal, emission.c_str(), source.c_str()); 
  });
  //// <--

  // this is the new name of the spec!
   mLayout->signal_callback_add("animation,end", "invisible", [this] (auto&& signal, auto&&
    emission, auto&& source) {
    this->invisibleFunc(signal, emission.c_str(), source.c_str()); 
  });

  mLayout->signal_callback_add("animation,end", "visible", [this] (auto&& signal, auto&&
    emission, auto&& source) {
    this->visibleFunc(signal, emission.c_str(), source.c_str()); 
  });
  // <-
 
  mLayout->signal_callback_add("*", "edje", [this] (auto&& signal, auto&&
    emission, auto&& source) {
    this->edjeFunc(signal, emission.c_str(), source.c_str()); 
  });
  
  mLayout->signal_callback_add("*", "stateval", [this] (auto&& signal, auto&&
    emission, auto&& source) {
    this->statevalFunc(signal, emission.c_str(), source.c_str()); 
  });
  
  mLayout->signal_callback_add("*", "*", [this] (auto&& signal, auto&&
    emission, auto&& source) {
    this->allFunc(signal, emission.c_str(), source.c_str()); 
  });
  
  mLayout->size_set(mEdjeContext->resolution);
  
  // initial screen widget update after realizing a screen
  updateDispatched(0);

  groupState = Realizing;

  mLayout->signal_emit("visible", "stateval");
    
  //mEdjeContext->background->hide (); // make background "transparent"

  mMutexRealize.lock();
  mCondRealize.signal();
  mMutexRealize.unlock();
  
  LOG4CXX_TRACE(mLogger, "-realizeDispatched()");
}

void EdjeView::unrealizeDispatched(int missedEvents)
{
  if (mLayout)
  {
    groupState = Unrealizing;
    //Eflxx::CountedPtr <Edjexx::Object> edjeObj = mLayout->getEdje();

    // show background while view switch to prevent flickering of 
    // below composite layer
    //mEdjeContext->background->show ();
    
    mLayout->signal_emit("invisible", "stateval");
  }

  for (WidgetIterator wl_it = beginOfWidgets();
       wl_it != endOfWidgets();
       ++wl_it)
  {
    Widget *w = wl_it->second;


    w->freeContent();
  }
}

void EdjeView::update()
{
  LOG4CXX_TRACE(mLogger, "+update()");

  mUpdateDispatcher.emit();

  LOG4CXX_TRACE(mLogger, "-update()");
}

void EdjeView::updateDispatched(int missedEvents)
{
  LOG4CXX_TRACE(mLogger, "updateDispatched()");
  
  for (WidgetIterator wl_it = beginOfWidgets();
       wl_it != endOfWidgets();
       ++wl_it)
  {
    Widget *w = wl_it->second;


    w->updateContent();
  }
}

void EdjeView::invisibleFunc(efl::layout::Signal s, const std::string emission, const std::string source)
{
  LOG4CXX_TRACE(mLogger, "invisibleFunc");

  groupState = Unrealized;
  //mWindow->delResizeObject(*mLayout);
  //mLayout->destroy();
  //mLayout = NULL;
  
  // signal the edje statemachine thread that the animation is finished
  mCondUnrealize.signal();
}

void EdjeView::visibleFunc(efl::layout::Signal s, const std::string emission, const std::string source)
{
  LOG4CXX_TRACE(mLogger, "visibleFunc");

  groupState = Realized;
}

void EdjeView::statevalFunc(efl::layout::Signal s, const std::string emission, const std::string source)
{
  LOG4CXX_TRACE(mLogger, "statevalFunc: " << emission << ", " << source);
}

void EdjeView::edjeFunc(efl::layout::Signal s, const std::string emission, const std::string source)
{
  LOG4CXX_TRACE(mLogger, "edjeFunc: " << emission << ", " << source);
}


void EdjeView::allFunc(efl::layout::Signal s, const std::string &emission, const std::string &source)
{  
  if (source != "stateval")
  {
    StateMachineAccessor &StateMachineAccessor(StateMachineAccessor::getInstance());

    string event("edje," + source + "," + emission);

    // => activate next line to get all events logged, but be warned as mouse events spam event queue much!
    LOG4CXX_DEBUG(mLogger, "allFunc: " << event << " (" << mGroupname << ")");

    // only push new events for realized screens
    // FIXME: when I do this it leads into freezes as the invisible signal doesn't come
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
    StateMachineAccessor &stateMachineAccessor(StateMachineAccessor::getInstance());

    // TODO: think about defining some common events (update/exit,...) in stateval lib 
    static const int VIEW_UPDATE_EVENT = stateMachineAccessor.findMapingEvent("VIEW_UPDATE");

    string eventString = stateMachineAccessor.findMapingEvent(mEvent);

    LOG4CXX_DEBUG(mLogger, "EdjeView::smEvents: " << mEvent << " / " << eventString);

    if ((eventString.length() >= 4) && (eventString.substr(4) != "edje"))
    {
      mLayout->signal_emit(eventString, "stateval");
    }

    if (mEvent == VIEW_UPDATE_EVENT)
    {
      update();
    }
  }

  mMutexPushEvent.lock();
  mCondPushEvent.signal ();
  mMutexPushEvent.unlock();
}

void EdjeView::pushEvent(int event)
{
  mEvent = event;
  
  mPushEventDispatcher.emit ();

  mMutexPushEvent.lock();  
  mCondPushEvent.wait(mMutexPushEvent);
  mMutexPushEvent.unlock();
}

Widget *EdjeView::createWidget(const std::string &name)
{
  Widget *widget = new EdjeWidget(*this, defaultWidgetRenderer, name);
  mWidgetMap[name] = widget;
  return widget;
}

efl::ui::Layout *EdjeView::getLayout()
{
  return mLayout;
}
