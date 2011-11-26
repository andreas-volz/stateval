#ifndef TRANSITION_H
#define TRANSITION_H

/* forward declarations */
class State;

class Transition
{
public:
  static const int NoEvent = -1;

  Transition(State *state);
  Transition(State *state, int event);
  virtual ~Transition();

  int getEvent() const;

  void setEndState(State *state);

  State *getEndState() const;

private:
  State *mEndState; // no need to free this as Loader holds it!
  int mEvent;
};

#endif // TRANSITION_H
