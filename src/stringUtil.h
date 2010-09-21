/*
 *  Copyright (C) 2005  Andreas Volz
 *  Copyright (C) 2006-2007  MakeHuman Project
 *
 *  This program is free software; you  can  redistribute  it  and/or
 *  modify  it  under  the terms of the GNU General Public License as
 *  published by the Free Software Foundation; either  version  3  of
 *  the License, or (at your option) any later version.
 *
 *  This  program  is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the  implied  warranty  of
 *  MERCHANTABILITY  or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
 *  General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program; if not, write to the Free Software Foun-
 *  dation, Inc., 59 Temple Place, Suite 330, Boston,  MA  02111-1307
 *  USA
 *
 *  File: util.h
 *
 */

#ifndef STRING_UTIL_H
#define STRING_UTIL_H

#ifdef HAVE_CONFIG_H
  #include <config.h>
#endif

/* STD */
#include <cstdlib>
#include <string> 
#include <sstream>
#include <iomanip>
#include <vector>
#include <iostream>

/* Project */
#include "../include/stateval/Exceptions.h"

/// some system specific defines
#if defined(WIN32) || defined(_WIN32) || defined(__WIN32__)
#  define PATH_SEPARATOR string("\\")
#else
#  define PATH_SEPARATOR string("/")
#endif

using std::string;
using std::vector;

// delete all following 'characters' in 'str'
void UtilStringDelFollow (std::string &str, const std::string &characters);

// delete all leading 'characters' in 'str'
void UtilStringDelLead (std::string &str, const std::string &characters);

// delete all surrounding 'characters' in 'str'
void UtilStringDelSurround (std::string &str, const std::string &characters);

// some functions that are very usefull for writing files
bool hasFileEnding (const std::string &filename, const std::string &ending);
std::string cutFileEnding (std::string filename, const std::string &ending = "");

// '<<' operator for vector class
/*template <typename T>
std::ostream &operator << (std::ostream &s, std::vector<T> iv)
{
  s << "[";
  for (unsigned i = 0; i < iv.size (); i++)
  {
    s << iv[i];
    if (i < iv.size ()-1)
      s << ",";
  }
  s << "]" << std::endl;

  return s;
}*/

/// print Vector on std::cout
template <typename T>
void printVector (std::vector<T> iv)
{
  std::cout << "[";
  for (unsigned i = 0; i < iv.size (); i++)
  {
    std::cout << iv[i];
    if (i < iv.size ()-1)
      std::cout << ",";
  }
  std::cout << "]" << std::endl;
}

/// create std::string from any number
template <typename T>
std::string toString (const T &thing, int w = 0, int p = 0)
{
  std::ostringstream os;
  os << std::setw(w) << std::setprecision(p) << thing;
  return os.str();

}

/*!
 * \brief Parse a string into tokens. 
 *
 * @line to extract tokens from
 * @seperator token separator characters
 * @num the maximum number of seperator matches (0 = no limit)
 * @return sequential string container
 */
template <typename T>
void stringToken (const std::string& line, const std::string &separator, int num, T& result)
{
  std::string::size_type start = line.find_first_not_of(separator);
  if (std::string::npos == start)
  {
    std::cout << "no " << separator << " found" << std::endl;
    return;  // nothing found
  }
  unsigned int i = 0;
  std::string::size_type last_end = std::string::npos;
  
  do
  {
    std::string::size_type end = line.find_first_of(separator,start);

    if ((num != i) || (num == 0))
    {
      if (end == std::string::npos) // copy complete string
      {
        result.push_back(line.substr(start)); // substr needed?
        start = std::string::npos;       // finish loop
      }
      else
      {
        result.push_back(line.substr(start, end - start));
        start = line.find_first_not_of(separator, end);
      }
    }
    else
    {
      result.push_back(line.substr(last_end + separator.length (), line.length ()));
      start = std::string::npos;       // finish loop
    }

    ++i;
    last_end = end;
  } while (std::string::npos != start);
}

int replaceString (const string& match, const string& replace, string& str, unsigned int maxReplace = 0);

// I make a helper function to compare string
bool compareCaseInsensitive(string strFirst, string strSecond);

/*! 
 * the third parameter of from_string() should be 
 * one of std::hex, std::dec (default) or std::oct
 */
template <class T>
T fromString (const std::string &s, 
              std::ios_base& (*f)(std::ios_base&) = std::dec)
{
  T t;
  std::istringstream iss(s);
  
  bool success = (iss >> f >> t);
  
  if (!success)
  {
    throw ConvertFromStringException (s + THROW_POS);
  }
  
  return t;
}



#endif	// STRING_UTIL_H
