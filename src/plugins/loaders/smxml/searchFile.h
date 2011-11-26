#ifndef SEARCHFILE_H
#define SEARCHFILE_H

#include <string>
#include <list>

// public search functions

/// ".so" is added....
const std::string searchPluginFile(const std::string &type, const std::string &name);

// private
const std::string searchFile(std::list <std::string> &nameList);

#endif // SEARCHFILE_H
