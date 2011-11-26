#ifndef VIEW_PLUGIN_LOADER_H
#define VIEW_PLUGIN_LOADER_H

/* pluxx */
#include "pluxx/PluginLoader.h"

/* STD */
#include <list>

/* local */
#include "stateval/Context.h"

class ViewPluginLoader : public pluxx::PluginLoader
{
public:
  static pluxx::Plugin *loadFactory(const std::string &filename, const std::string &type, unsigned int majorVersion,
                                    Context *context, const std::list <std::string> &params);

private:

};

#endif // VIEW_PLUGIN_LOADER_H
