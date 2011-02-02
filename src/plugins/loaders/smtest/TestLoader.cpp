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
  }
  else if (test == "TEST2")
  {
    // test a special statemachine with test 2
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
