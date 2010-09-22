#ifndef MAIN_H
#define MAIN_H

/* EFL */
#include <ecorexx/Ecorexx.h>

class Main
{
public:
  Main (int argc, const char **argv);
  
private:
  void hideWindow (const Ecorexx::EvasWindow &win);
  void resizeWindow (const Ecorexx::EvasWindow &win);
  void quit ();

  Ecorexx::Application mApp;
  Ecorexx::EvasWindowSoftwareX11 mWindow;
  Evasxx::Rectangle mBackgroundRect;
};

#endif // MAIN_H
