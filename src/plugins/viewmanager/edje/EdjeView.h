#ifndef EDJE_VIEW_H
#define EDJE_VIEW_H

/* stateval */
#include "stateval/stateval.h"
#include "stateval/private/stateval_private.h"
#include "stateval/private/Logger.h"

/* EFLxx */
#include <Efl_Ui.hh>

/* local */
#include "EcoreDispatcher.h"
#include "EdjeDefaultWidgetRenderer.h"

/* STD */
#include <string>

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

  efl::ui::Layout *getLayout();  

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

  void invisibleFunc(efl::layout::Signal s, const std::string emission, const std::string source);
  void visibleFunc(efl::layout::Signal s, const std::string emission, const std::string source);
  void statevalFunc(efl::layout::Signal s, const std::string emission, const std::string source);
  void edjeFunc(efl::layout::Signal s, const std::string emission, const std::string source);
  void allFunc(efl::layout::Signal s, const std::string &emission, const std::string &source);
  void viewUpdateFunc(const std::string emission, const std::string source);

  /** Variables **/
  Logger mLogger;

  EdjeContext *mEdjeContext;

  efl::ui::Layout *mLayout;
  efl::ui::Win *mWindow;

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

  EdjeDefaultWidgetRenderer defaultWidgetRenderer;
};

#endif // EDJE_VIEW_H
