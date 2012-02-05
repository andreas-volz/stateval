#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

/* Project */
#include <stateval/stateval.h>
#include "searchFile.h"
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

Main::Main()
{
#ifdef HAVE_LOG4CXX
  PropertyConfigurator::configure(searchDataDir() + "/logging.prop");
#endif // HAVE_LOG4CXX

  StateMachineAccessor &stateMachineAccessor(StateMachineAccessor::getInstance());
  stateMachineAccessor.load("smxml", searchDataDir() + "/text_smxml/test.smxml");

  stateMachineAccessor.run();

  // TODO: don't do this in main, but in plugin!
  stateMachineAccessor.pushEvent("HK_NAV");

  StateMachineAccessor::destroy();
}

int main(int argc, char **argv)
{
  Main mainobject;

  return 0;
}
