#ifndef VIEW_STATE_H
#define VIEW_STATE_H

/* local */
#include "SimpleState.h"
#include "View.h"
#include "Logger.h"

/* STD */
#include <list>

/* forward declarations */
class ViewCache;
class ViewSpec;

class ViewState : public SimpleState
{
public:
  ViewState(CompoundState *parentState, ViewCache *viewCache);
  ~ViewState();

  void addView(View &view, int layer);

protected:
  void pushEvent(int event);

private:
  void beforeTransitionCode();
  void afterTransitionCode();

  bool hasView();

  void mapEvent(int &inOutEvent);

  std::list <ViewSpec> mViewList;
  ViewCache *mViewCache;

  Logger mLogger;
};

#endif // VIEW_STATE_H
