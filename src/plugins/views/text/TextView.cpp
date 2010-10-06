#ifdef HAVE_CONFIG_H
  #include <config.h>
#endif

/* Project */
#include "TextView.h"
#include "FileReader.h"

/* STD */
#include <iostream>

using namespace std;

TextView::TextView (const std::string &fileName) :
  mFileName (fileName)
{
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
