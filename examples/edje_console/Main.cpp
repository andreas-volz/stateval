#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

/* STD */
#include <iostream>
#include <string>
#include <map>
#include <cassert>

/* local */
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

Main::Main(int argc, const char **argv)
{
#ifdef HAVE_LOG4CXX
  //BasicConfigurator::configure ();
  cout << "searchDataDir ():" << searchDataDir() << endl;
  PropertyConfigurator::configure(searchDataDir() + "/logging.prop");
#endif // HAVE_LOG4CXX

  StateMachineAccessor &StateMachineAccessor(StateMachineAccessor::getInstance());
  StateMachineAccessor.load("smxml", searchDataDir() + "/mobile_smxml/mobile.smxml");

  StateMachineAccessor.start();

  // create an input thread
  //InputThread iThread(StateMachineAccessor);
  //iThread.start();

  // stupid wait loop
  for (;;)
  {
    sleep(1);
  }

  StateMachineAccessor::destroy();
  // <-- EFL
}

/* Main */
int main(int argc, const char **argv)
{
  Main mainobject(argc, argv);

  return 0;
}
