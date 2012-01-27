#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

/* local */
#include "TextView.h"
#include "FileReader.h"

/* stateval */

/* STD */
#include <iostream>
#include <cassert>

using namespace std;

TextView::TextView(const std::map <std::string, std::string> &params)
{
  std::map <std::string, std::string>::const_iterator param_it = params.find ("filename");
  if (param_it != params.end ())
  {
    mFileName = param_it->second;
  }
  else
  {
    // TODO: needed error handling
    assert (false);      
  }
  
}

void TextView::realize()
{
  FileReader fileReader;
  fileReader.open(mFileName);

  cout << "reading " << mFileName << "..." << endl;

  string str;
  while (getline(fileReader, str))
  {
    cout << str << endl;
  }
}

void TextView::unrealize()
{
  cout << "*************************[clear]" << endl;
}

void TextView::updateContent()
{
  cout << "*************************[update]" << endl;
  // as update isn't good possible on text console, simply realize it again...
  realize();
}
