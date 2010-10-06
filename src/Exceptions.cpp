#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

/* STD */
#include <string>

/* Project */
#include "stateval/Exceptions.h"

using namespace std;

const char* FileNotFoundException::what () const throw ()
{
  static string s;
  s = "File not found: '";
  s += txt;
  s += "'.";

  return static_cast <const char*> (s.c_str ());
}

const char* ConvertFromStringException::what () const throw ()
{
  static string s;
  s = "Coudn't convert '";
  s += txt;
  s += "' to number..";

  return static_cast <const char*> (s.c_str ());
}