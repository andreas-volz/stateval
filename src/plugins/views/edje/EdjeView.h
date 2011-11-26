#ifndef EDJE_VIEW_H
#define EDJE_VIEW_H

/* stateval */
#include "stateval/stateval.h"
#include "stateval/private/stateval_private.h"

/* local */
#include "EcoreDispatcher.h"
#include "Logger.h"

/* STD */
#include <string>

/* EFLxx */
#include <evasxx/Evasxx.h>
#include <edjexx/Edjexx.h>
#include <elementaryxx/Elementaryxx.h>

/* forward declarations */
class StateMachineAccessor;
class EdjeContext;

static bool initialized = false;

class EdjeView : public View
{
public:
  EdjeView (Context *context, const std::list <std::string> &params);

  const std::string getType ();
  
  const unsigned int getMajorVersion ();

  const unsigned int getMinorVersion ();
  
  void realize ();
  void unrealize ();

  void updateContent ();

  void pushEvent (int event);
  
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
  void statevalFunc (const std::string emmision, const std::string source);
  void edjeFunc (const std::string emmision, const std::string source);
  void allFunc (const std::string emmision, const std::string source);
  void viewUpdateFunc (const std::string emmision, const std::string source);
  
  /** Variables **/
  Logger mLogger;
  
  EdjeContext *mEdjeContext;
  
  Elmxx::Layout *mLayout;
  Elmxx::Window *mWindow;

  std::string mFilename;
  std::string mGroupname;
  
  EcoreDispatcher mRealizeDispatcher;
  EcoreDispatcher mUnrealizeDispatcher;
  
  Threading::Condition condUnrealize;
  Threading::Mutex mutexUnrealize;

  Threading::Condition condRealize;
  Threading::Mutex mutexRealize;
  
  enum ViewState groupState;
};

#endif // EDJE_VIEW_H
