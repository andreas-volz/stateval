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
  EdjeView(EdjeContext *context, const std::string &dir, const std::map <std::string, std::string> &params);

  void realize();
  void unrealize();
  void update();
  
  void pushEvent(int event);

  Widget *createWidget(const std::string &name);

  // FIXME: only set to public to implement widget
  Elmxx::Layout *mLayout; // TODO: remove and access mEdjeContext

private:
  enum ViewState
  {
    Unrealized,
    Realized,
    Realizing,
    Unrealizing
  };

  void realizeDispatched(int missedEvents);
  void updateDispatched(int missedEvents);
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

  
  Elmxx::Window *mWindow; // TODO: remove and access mEdjeContext

  std::string mFilename;
  std::string mGroupname;

  EcoreDispatcher mUnrealizeDispatcher;
  Threading::Condition mCondUnrealize;
  Threading::Mutex mMutexUnrealize;

  EcoreDispatcher mRealizeDispatcher;
  Threading::Condition mCondRealize;
  Threading::Mutex mMutexRealize;

  EcoreDispatcher mPushEventDispatcher;
  Threading::Condition mCondPushEvent;
  Threading::Mutex mMutexPushEvent;

  EcoreDispatcher mUpdateDispatcher;

  enum ViewState groupState;

  // dispatched by mPushEventDispatcher
  int mEvent;
};

#endif // EDJE_VIEW_H
