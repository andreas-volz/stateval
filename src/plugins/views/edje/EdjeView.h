#ifndef EDJE_VIEW_H
#define EDJE_VIEW_H

/* stateval */
#include "stateval/stateval.h"

/* local */
#include "EcoreDispatcher.h"

/* STD */
#include <string>

/* EFL */
#include <evasxx/Evasxx.h>
#include <edjexx/Edjexx.h>

/* forward declarations */
class StateMachineAccess;
class EdjeContext;

class EdjeView : public View
{
public:
  EdjeView (Context *context, const std::list <std::string> &params);

  const std::string getType ();
  
  const unsigned int getMajorVersion ();

  const unsigned int getMinorVersion ();

  void updateContent ();
  
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

  void smEvents (int event);
  
  /** Variables **/
  
  /* Singletons at first */
  StateMachineAccess *mStateMachineAccess;
  EdjeContext *mEdjeContext;
  
  Evasxx::Canvas *mEvas;
  Edjexx::Object *mEdje;
  std::string mFilename;
  std::string mGroupname;
  
  EcoreDispatcher mRealizeDispatcher;
  EcoreDispatcher mUnrealizeDispatcher;
  
  Threading::Condition condUnrealize;
  Threading::Mutex mutexUnrealize;
  
  enum ViewState groupState;

  Edjexx::ExternalParam param;
};

#endif // EDJE_VIEW_H
