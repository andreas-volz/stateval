#ifndef VIEW_MANAGER_PLUGIN_LOADER_H
#define VIEW_MANAGER_PLUGIN_LOADER_H

/* pluxx */
#include "pluxx/PluginLoader.h"

/* STD */
#include <map>

class ViewManagerPluginLoader : public pluxx::PluginLoader
{
public:
  static pluxx::Plugin *loadFactory(const std::string &filename, const std::string &type, unsigned int majorVersion,
                                    const std::map <std::string, std::string> &params);

private:

};

#endif // VIEW_MANAGER_PLUGIN_LOADER_H
