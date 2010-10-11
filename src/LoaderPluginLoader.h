#ifndef LOADER_PLUGIN_LOADER_H
#define LOADER_PLUGIN_LOADER_H

class LoaderPluginLoader
{
public:
  static pluxx::Plugin *loadFactory (const std::string &filename, const std::string& type, unsigned int majorVersion,
                                     Context *context);
  
private:
};

#endif // LOADER_PLUGIN_LOADER_H
