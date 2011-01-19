#ifndef VIEW_STATE_H
#define VIEW_STATE_H

/* Project */
#include "SimpleState.h"
#include "View.h"

class ViewState : public SimpleState
{
public:
  ViewState (CompoundState *parentState, View &view);

protected:
  void pushEvent (int event);
  
private:
  void beforeTransitionCode ();
  void afterTransitionCode ();

  // TODO: think about how usefull it is to have a ViewState without view.
  // maybe a better alternative would here to detect this at loading and exit...
  bool hasView ();
  
  View *getView ();
  
  void mapEvent (int &inOutEvent);

  // this memory doesn't need to be freed as it's handled in the Loader!
  View *mView;
};

#endif // VIEW_STATE_H
