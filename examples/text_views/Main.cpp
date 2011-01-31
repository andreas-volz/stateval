#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

/* Project */
#include <stateval/stateval.h>
#include "searchFile.h"
#include "InputThread.h"
#include "Main.h"

/* log4cxx */
#ifdef HAVE_LOG4CXX
#include "log4cxx/logger.h"
#include "log4cxx/basicconfigurator.h"
#include "log4cxx/propertyconfigurator.h"
#include "log4cxx/helpers/exception.h"
#endif // HAVE_LOG4CXX

using namespace std;

#ifdef HAVE_LOG4CXX
using namespace log4cxx;
using namespace log4cxx::helpers;
#endif // HAVE_LOG4CXX

Main::Main ()
{
#ifdef HAVE_LOG4CXX
  PropertyConfigurator::configure (searchDataDir () + "/logging.prop");
#endif // HAVE_LOG4CXX
  
  StateMachineAccessor &StateMachineAccessor (StateMachineAccessor::getInstance ());
  StateMachineAccessor.load ("smxml", searchDataDir () + "/text_smxml/test.smxml", NULL);

  StateMachineAccessor.start ();
  
  // create an input thread
  // yes, I know an input thread isn't really needed here.
  // but as it was done like this in the edje_console example...
  InputThread iThread (StateMachineAccessor);
  iThread.start ();
  
  // inital event
  StateMachineAccessor.pushEvent ("HK_NAV");

  // stupid wait loop
  for (;;)
  {
    sleep (1);
  }

  StateMachineAccessor::destroy ();
}

int main (int argc, char **argv)
{
  Main mainobject;
  
  return 0;
}
