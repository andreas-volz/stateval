#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

/* Project */
#include <stateval/stateval.h>
#include "searchFile.h"
#include "InputThread.h"
#include "Main.h"

#ifdef HAVE_LOG4CXX
/* log4cxx */
#include <log4cxx/logger.h>
#include "log4cxx/basicconfigurator.h"
#endif // HAVE_LOG4CXX

using namespace std;

#ifdef HAVE_LOG4CXX
using namespace log4cxx;
using namespace log4cxx::helpers;
#endif // HAVE_LOG4CXX

Main::Main ()
{
#ifdef HAVE_LOG4CXX
  BasicConfigurator::configure();
#endif // HAVE_LOG4CXX
  
  StateMachine sm ("smxml");
  
  //sm.load (NULL, searchDataDir () + "/text_sm/");
  sm.load (NULL, searchDataDir () + "/text_smxml/test.smxml");
  
  StateMachineThread smThread (sm);
  
  smThread.start ();
  
  StateMachineAccess &stateMachineAccess (StateMachineAccess::instance ());
  stateMachineAccess.init (sm, smThread);

  // create an input thread
  // yes, I know an input thread isn't really needed here.
  // but as it was done like this in the edje_console example...
  InputThread iThread (stateMachineAccess);
  iThread.start ();
  
  sm.start ();
  
  // inital event
  stateMachineAccess.pushEvent ("HK_NAV");

  // stupid wait loop
  for (;;)
  {
    sleep (1);
  }
}

int main (int argc, char **argv)
{
  Main mainobject;
  
  return 0;
}
