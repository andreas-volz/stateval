#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

/* STD */
#include <iostream>
#include <string>
#include <map>
#include <cassert>
#include <cstdlib>

/* local */
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

Main::Main(int argc, const char **argv)
{
#ifdef HAVE_LOG4CXX
  //BasicConfigurator::configure ();
  cout << "searchDataDir ():" << searchDataDir() << endl;
  PropertyConfigurator::configure(searchDataDir() + "/logging.prop");
#endif // HAVE_LOG4CXX

  string pluginType ("smxml");
  string file;
  
  if (argc == 3)
  {
    pluginType = argv[1];
    file = argv[2];
  }
  else if (argc == 2)
  {
    file = argv[1];
  }
  else
  {
    cerr << "Please give one or two parameters:" << endl << endl;
    cerr << "stateval_player [<plugin>] <file>" << endl;
    cerr << endl;
    exit (1);
  }

  StateMachineAccessor &StateMachineAccessor(StateMachineAccessor::getInstance());
  StateMachineAccessor.load(pluginType, file);

  StateMachineAccessor.run();

  StateMachineAccessor::destroy();

  cout << "Exit!" << endl;
}

/* Main */
int main(int argc, const char **argv)
{
  Main mainobject(argc, argv);

  return 0;
}
