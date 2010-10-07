#ifdef HAVE_CONFIG_H
  #include <config.h>
#endif

/* Project */
#include "TextView.h"
#include "FileReader.h"

/* STD */
#include <iostream>

using namespace std;

static const char* type = "View";
static const unsigned int major_version = 1;
static const unsigned int minor_version = 1;

TextView::TextView (const std::list <std::string> &params)
{
  //if (params.length () != 1)
    //throw something

  std::list <std::string>::const_iterator params_it = params.begin ();
    
  mFileName = *params_it;
}

const std::string TextView::getType ()
{
  return type;
}

const unsigned int TextView::getMajorVersion ()
{
  return major_version;
}

const unsigned int TextView::getMinorVersion ()
{
  return minor_version;
}

void TextView::realize ()
{
  FileReader fileReader;
  fileReader.open (mFileName);

  cout << "reading " << mFileName << "..." << endl;

  string str;
  while (getline (fileReader, str))
  {
    cout << str << endl;
  }
}

void TextView::unrealize ()
{
  cout << "*************************[clear]" << endl;
}


/*****************************/
/* Plugin needed C functions */
/*****************************/

PLUGIN_EXPORT TextView *plugin_create (const std::list <std::string> &params)
{
  // FIXME: think about pluxx change or define init function in EdjeView
  return new TextView (params);
}

PLUGIN_EXPORT void plugin_destroy (View *plugin)
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
