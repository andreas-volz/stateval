/* local */
#include "ViewPluginLoader.h"

using namespace pluxx;

typedef Plugin *plugin_create_t (const std::list <std::string> &params);

Plugin *LoaderPluginLoader::loadFactory(const std::string &filename, const std::string &type, unsigned int majorVersion,
                                        Context *context)
{
  void *pluginHandle = loadFactoryInternal(filename, type, majorVersion);

  plugin_create_t *plugin_create = (plugin_create_t *) loadSymbol(pluginHandle, "plugin_create");

  Plugin *plugin = plugin_create(params);
  registerPlugin(plugin, pluginHandle);

  return plugin;
}
