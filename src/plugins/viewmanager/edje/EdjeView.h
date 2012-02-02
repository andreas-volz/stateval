#ifndef EDJE_VIEW_H
#define EDJE_VIEW_H

/* stateval */
#include "stateval/stateval.h"
#include "stateval/private/stateval_private.h"
#include "stateval/private/Logger.h"

/* local */
#include "EcoreDispatcher.h"

/* STD */
#include <string>

/* EFLxx */
#include <evasxx/Evasxx.h>
#include <edjexx/Edjexx.h>
#include <elementaryxx/Elementaryxx.h>

/* forward declarations */
class StateMachineAccessor;
class EdjeContext;

class EdjeView : public View
{
public:
  EdjeView(EdjeContext *context, const std::map <std::string, std::string> &params);


  void realize();
  void unrealize();

  void updateContent();

  void pushEvent(int event);

private:
  enum ViewState
  {
    Unrealized,
    Realized,
    Realizing,
    Unrealizing
  };

  void realizeDispatched(int missedEvents);
  void unrealizeDispatched(int missedEvents);

  void pushEventDispatched(int missedEvents);

  void invisibleFunc(const std::string emmision, const std::string source);
  void visibleFunc(const std::string emmision, const std::string source);
  void statevalFunc(const std::string emmision, const std::string source);
  void edjeFunc(const std::string emmision, const std::string source);
  void allFunc(const std::string emmision, const std::string source);
  void viewUpdateFunc(const std::string emmision, const std::string source);

  /** Variables **/
  Logger mLogger;

  EdjeContext *mEdjeContext;

  Elmxx::Layout *mLayout; // TODO: remove and access mEdjeContext
  Elmxx::Window *mWindow; // TODO: remove and access mEdjeContext

  std::string mFilename;
  std::string mGroupname;

  EcoreDispatcher mRealizeDispatcher;
  EcoreDispatcher mUnrealizeDispatcher;

  EcoreDispatcher mPushEventDispatcher;

  Threading::Condition condUnrealize;
  Threading::Mutex mutexUnrealize;

  Threading::Condition condRealize;
  Threading::Mutex mutexRealize;

  Threading::Condition mCondPushEvent;
  Threading::Mutex mMutexPushEvent;

  enum ViewState groupState;

  // dispatched by mPushEventDispatcher
  int mEvent;
};

#endif // EDJE_VIEW_H
