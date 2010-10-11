#ifndef EDJE_CONTEXT_H
#define EDJE_CONTEXT_H

/* EFL */
#include <evasxx/Evasxx.h>

/* pluxx */
#include "stateval/stateval.h"

class EdjeContext : public Context
{
public:
  EdjeContext (Evasxx::Canvas &evas)  :
    mEvas (&evas) {}
  virtual ~EdjeContext () {}

  void setResolution (const Eflxx::Size &resolution) {mResolution = resolution;}

  const Eflxx::Size getResolution () {return mResolution;}
  
  Evasxx::Canvas &getCanvas () {return *mEvas;}
  
private:
  Evasxx::Canvas *mEvas;
  Eflxx::Size mResolution;
};

#endif // EDJE_CONTEXT_H
