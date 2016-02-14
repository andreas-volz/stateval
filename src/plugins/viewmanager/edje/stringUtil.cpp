#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include <cctype>
#include <algorithm>

#include "stringUtil.h"

using namespace std;

void UtilStringDelFollow(std::string &str, const std::string &characters)
{
  string::size_type pos = str.find_last_not_of(characters);
  if (string::npos != pos)
    str = str.substr(0, pos + 1);
  else
  {
    // it is still possible that 'str' contains only 'characters':
    if (string::npos != str.find_first_of(characters))
      str.erase();
  }

}

void UtilStringDelLead(std::string &str, const std::string &characters)
{
  string::size_type pos = str.find_first_not_of(characters);
  if (string::npos != pos)
    str = str.substr(pos);
  else
  {
    // it is still possible that 'str' contains only 'characters':
    if (string::npos != str.find_first_of(characters))
      str.erase();
  }

}

void UtilStringDelSurround(std::string &str, const std::string &characters)
{
  string::size_type pos = str.find_first_not_of(characters);
  if (string::npos != pos)
    str = str.substr(pos);

  pos = str.find_last_not_of(characters);
  if (string::npos != pos)
    str = str.substr(0, pos + 1);
  else
  {
    // it is still possible that 'str' contains only 'characters':
    if (string::npos != str.find_first_of(characters))
      str.erase();
  }
}

bool hasFileEnding(const std::string &filename, const std::string &ending)
{
  const size_t loc = filename.find(ending, filename.length() - ending.length());

  if (loc != string::npos)
  {
    return true;
  }

  return false;
}

std::string cutFileEnding(std::string filename, const std::string &ending)
{
  if (ending == "")
  {
    const size_t loc = filename.find_last_of('.', filename.length());

    if (loc != string::npos)
    {
      filename.erase(loc);
      return filename;
    }
  }
  else
  {
    const size_t loc = filename.find(ending, filename.length() - ending.length());

    if (loc != string::npos)
    {
      filename.erase(loc);
      return filename;
    }
  }

  return filename;
}

/*!
 * An function to replace occurences of substrings in a bigger string. Very basic function without regex.
 * @param match This is the string that is matched to replace.
 * @param replace The string that replaces all found match strings.
 * @param str The string that is matched and replaced.
 * @param maxReplace Give a int to limit the replace matches. If 0 is given there's no limit.
 */
int replaceString(const string &match, const string &replace, string &str, unsigned int maxReplace)
{
  int start = 0;
  unsigned int i = 0;

  if (maxReplace == 0)
    maxReplace = str.length();

  for (; i < maxReplace; i++)
  {
    string::size_type loc = str.find(match, start);

    if (loc != string::npos)
    {
      str.replace(loc, match.length(), replace);
    }
    else
    {
      return i;
    }

    start = loc;
  }

  return i;
}

/*!
 * Convert both strings to upper case by transfrom() before compare.
 * @todo optimization possible with compare of each char after toupper; may help for very long strings
 */
bool compareCaseInsensitive(std::string strFirst, std::string strSecond)
{
  std::transform(strFirst.begin(), strFirst.end(), strFirst.begin(), ::toupper);
  std::transform(strSecond.begin(), strSecond.end(), strSecond.begin(), ::toupper);

  if (strFirst == strSecond)
    return true;
  else
    return false;
}
