/* local */
#include "ViewManagerPluginLoader.h"

using namespace pluxx;

typedef Plugin *plugin_create_t (const std::map <std::string, std::string> &params);

Plugin *ViewManagerPluginLoader::loadFactory(const std::string &filename, const std::string &type, unsigned int majorVersion,
                                      const std::map <std::string, std::string> &params)
{
  void *pluginHandle = loadFactoryInternal(filename, type, majorVersion);

  plugin_create_t *plugin_create = (plugin_create_t *) loadSymbol(pluginHandle, "plugin_create");

  Plugin *plugin = plugin_create(params);
  registerPlugin(plugin, pluginHandle);

  return plugin;
}
