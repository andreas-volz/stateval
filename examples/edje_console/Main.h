#ifndef MAIN_H
#define MAIN_H

/* EFL */
#include <ecorexx/Ecorexx.h>
#include <elementaryxx/Elementaryxx.h>

class Main
{
public:
  Main(int argc, const char **argv);

private:
  void elm_quit(Evasxx::Object &obj, void *event_info);
  void startupDispatched();

  Elmxx::Application mApp;
  Elmxx::Window *mWindow;
  Elmxx::Background *mBackground;
};

#endif // MAIN_H
