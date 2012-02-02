#ifndef EDJE_CONTEXT_H
#define EDJE_CONTEXT_H

/* Eflxx */
#include <evasxx/Evasxx.h>
#include <elementaryxx/Elementaryxx.h>

/* pluxx */
#include "stateval/stateval.h"

struct EdjeContext 
{
  Elmxx::Window *window;
  Eflxx::Size resolution;
  Elmxx::Background *background;
};

#endif // EDJE_CONTEXT_H

