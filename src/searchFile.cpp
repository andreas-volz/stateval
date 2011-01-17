#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include <sys/types.h>
#include <sys/stat.h>
#include <iostream>
#include "searchFile.h"
#include "stateval/Exceptions.h"

using namespace std;

const std::string searchPluginFile (const std::string &type, const std::string &name)
{
  list <string> nameList; // TODO: port to list and iterator usage
  const string ext (".so");
  const string relPath ("plugins/" + type + "/" + name + "/");
  const string soName ("stateval_" + type + "_" + name + ext);
  
  // as default search only on the current directory
  nameList.push_back (relPath + soName);
  
#ifdef HAVE_CONFIG_H
  nameList.push_back (string (PACKAGE_PLUGINS_DIR) + "/" + relPath + soName);
  nameList.push_back (string (PACKAGE_SOURCE_DIR) + "/src/" + relPath + LT_OBJDIR + soName);
#endif
  
  const string &file = searchFile (nameList);

  if (file.empty ())
  {
    throw FileNotFoundException (relPath + soName);
  }

  return file;
}

const std::string searchDataDir ()
{
  list <string> nameList;

  nameList.push_back ("data");
  
#ifdef HAVE_CONFIG_H
  nameList.push_back (string (PACKAGE_SOURCE_DIR) + "/data");
  nameList.push_back (PACKAGE_DATA_DIR);
#endif
  
  return searchFile (nameList);
}

const std::string searchFile (std::list <std::string> &nameList)
{
  struct stat buf;

  for (std::list <std::string>::const_iterator li_it = nameList.begin ();
       li_it != nameList.end ();
       ++li_it)
  {
    const string &try_name = *li_it;

    bool found = !(stat (try_name.c_str (), &buf));
    //cout << "try_name: " << try_name << endl;

    if (found)
    {
      return try_name;
    }
  }

  return "";
}
