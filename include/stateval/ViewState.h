#ifndef VIEW_STATE_H
#define VIEW_STATE_H

/* Project */
#include "SimpleState.h"
#include "View.h"

class ViewState : public SimpleState
{
public:
  ViewState (State *parentState, View &view);
    
private:
  void beforeTransitionCode ();
  void afterTransitionCode ();
  
  bool hasView ();
  View *getView ();
  
  void mapEvent (int &inOutEvent);
  
  View *mView; // TODO: free
};

#endif // VIEW_STATE_H
