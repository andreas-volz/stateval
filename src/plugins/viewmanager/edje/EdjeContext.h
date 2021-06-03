#ifndef EDJE_CONTEXT_H
#define EDJE_CONTEXT_H

/* EFL */
#include <Efl_Ui.hh>

/* pluxx */
#include "stateval/stateval.h"

struct EdjeContext 
{
  efl::ui::Win *window;
  Eina_Size2D resolution;
  efl::ui::Bg *background;
};

#endif // EDJE_CONTEXT_H

