#ifndef EDJE_VIEW_H
#define EDJE_VIEW_H

/* Project */
#include "View.h"
#include "EcoreDispatcher.h"
#include "GraphicContext.h"

/* STD */
#include <string>

/* EFL */
#include <evasxx/Evasxx.h>
#include <edjexx/Edjexx.h>

/* GLIB */
#include <glibmm/thread.h>

/* forward declarations */
class StateMachineAccess;

class EdjeView : public View
{
public:
  EdjeView (const std::string &filename, const std::string &groupname);
  
  void realize ();
  void unrealize ();
  
private:
  enum ViewState
  {
    Unrealized,
    Realized,
    Realizing,
    Unrealizing
  };
  
  void realizeDispatched (int missedEvents);
  void unrealizeDispatched (int missedEvents);
  
  void invisibleFunc (const std::string emmision, const std::string source);
  void visibleFunc (const std::string emmision, const std::string source);
  void frameworkFunc (const std::string emmision, const std::string source);
  void edjeFunc (const std::string emmision, const std::string source);
  void allFunc (const std::string emmision, const std::string source);
  
  /** Variables **/
  
  /* Singletons at first */
  StateMachineAccess *mStateMachineAccess;
  GraphicContext *mGraphicContext;
  
  Evasxx::Canvas *mEvas;
  Edjexx::Object *mEdje;
  const std::string mFilename;
  const std::string mGroupname;
  
  EcoreDispatcher mRealizeDispatcher;
  EcoreDispatcher mUnrealizeDispatcher;
  
  Glib::Cond condUnrealize;
  Glib::Mutex mutexUnrealize;
  
  enum ViewState groupState;
};

#endif // EDJE_VIEW_H
