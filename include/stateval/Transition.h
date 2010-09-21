#ifndef TRANSITION_H
#define TRANSITION_H

/* forward declarations */
class State;

class Transition
{
public:
  Transition (State *state);
  Transition (State *state, int event);
  virtual ~Transition ();

  int getEvent () const;

  void setEndState (State *state);

  State *getEndState () const;
  
private:
  State *mEndState;
  int mEvent;
};

#endif // TRANSITION_H
