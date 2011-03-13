#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

/* local */
#include "TestLoader.h"

/* STD */
#include <cassert>

using namespace std;

static const char* type = "Loader";
static const unsigned int major_version = 1;
static const unsigned int minor_version = 1;

TestLoader::TestLoader () :
  mLogger ("stateval.plugins.loaders.smtest"),
  mContext (NULL)
{
}

TestLoader::~TestLoader ()
{
}

const std::string TestLoader::getType ()
{
  return type;
}

const unsigned int TestLoader::getMajorVersion ()
{
  return major_version;
}

const unsigned int TestLoader::getMinorVersion ()
{
  return minor_version;
}

bool TestLoader::load (Context *context, const std::string &test)
{
  mContext = context;
  
  LOG4CXX_INFO (mLogger, "load: " << test);

  // new test logic...
  // TODO: create test cases
  // TODO: create eacg test case as class derived from a test base class...
  if (test == "TEST1")
  {
    // test a special statemachine with test 1
    // description: easiest case ever: two simple states with a default transition
    SimpleState *simlpeState1 = new SimpleState (NULL);
    SimpleState *simlpeState2 = new SimpleState (NULL);
    
    addState (simlpeState1);
    addState (simlpeState2);

    Transition *trans = new Transition (simlpeState2);
    simlpeState1->addLeaveTransition (*trans);
    
    // TODO: -> to state should be a special test ViewState as this is the only 
    //          way to get a result currently
  }
  else if (test == "TEST2")
  {
    // test a special statemachine with test 2
    CompoundState *rootState = new CompoundState (NULL); // create a ROOT node
    addState (rootState);
    // ...
  }
  // ...{}
}


/*****************************/
/* Plugin needed C functions */
/*****************************/

PLUGIN_EXPORT TestLoader *plugin_create ()
{
  return new TestLoader ();
}

PLUGIN_EXPORT void plugin_destroy (Loader *plugin)
{
  delete plugin;
}

PLUGIN_EXPORT const char *get_plugin_type ()
{
  return type;
}

PLUGIN_EXPORT unsigned int get_plugin_major_version ()
{
  return major_version;
}
