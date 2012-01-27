#ifndef EDJE_CONTEXT_H
#define EDJE_CONTEXT_H

/* Eflxx */
#include <evasxx/Evasxx.h>
#include <elementaryxx/Elementaryxx.h>

/* pluxx */
#include "stateval/stateval.h"

class EdjeContext 
{
public:
  EdjeContext(Elmxx::Window *window)  :
    mWindow(window) {}
  virtual ~EdjeContext() {}

  void setResolution(const Eflxx::Size &resolution)
  {
    mResolution = resolution;
  }

  const Eflxx::Size getResolution()
  {
    return mResolution;
  }

  Elmxx::Window *getWindow()
  {
    return mWindow;
  }

private:
  Elmxx::Window *mWindow;
  Eflxx::Size mResolution;
};

#endif // EDJE_CONTEXT_H
