/* STD */
#include <iostream>
#include <string>

/* XML */

#include <libxml++/libxml++.h>

/* stateval */
#include "stateval/State.h"
#include "stateval/Loader.h"

class XMLLoader : public Loader
{
public:  
  XMLLoader ();
  ~XMLLoader ();

  const std::string getType ();
  
  const unsigned int getMajorVersion ();

  const unsigned int getMinorVersion ();
  
  bool load (Context *context, const std::string &smDir);
  
  void unload ();
    
protected:  
  void parseRootNode (const xmlpp::Node* node);
  void parseBlockNode (const xmlpp::Node* node);
  
  void parseEventsNode (const xmlpp::Node* node);
  void parseEventNode (const xmlpp::Node* node);

  void parseStatesNode (const xmlpp::Node* node);
  void parseStateNode (const xmlpp::Node* node);

  void parseTransitionsNode (const xmlpp::Node* node);
  void parseTransitionNode (const xmlpp::Node* node);

  void parseViewsNode (const xmlpp::Node* node);
  void parseViewNode (const xmlpp::Node* node);
  void parseViewMapNode (const xmlpp::Node* node);

  void loadSiteFile (const Glib::ustring& filepath);
  
private:
  const std::string rootNode;
  const std::string siteNode;
  const std::string folderNode;
};
