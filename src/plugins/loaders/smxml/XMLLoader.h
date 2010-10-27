/* STD */
#include <iostream>
#include <string>

/* XML */

#include <libxml++/libxml++.h>

/* stateval */
#include "stateval/stateval.h"

class XMLLoader : public Loader
{
public:  
  XMLLoader ();
  ~XMLLoader ();

  const std::string getType ();
  
  const unsigned int getMajorVersion ();

  const unsigned int getMinorVersion ();
  
  bool load (Context *context, const std::string &sm);
  
  void unload ();
    
protected:  
  void parseRootNode (const xmlpp::Node* node);
  void parseBlockNode (const xmlpp::Node* node);
  
  void parseEventsNode (const xmlpp::Node* node);
  void parseEventNode (const xmlpp::Node* node);

  void parseActionsNode (const xmlpp::Node* node);
  void parseActionNode (const xmlpp::Node* node);

  void parseStatesNode (const xmlpp::Node* node);
  void parseStateNodeIndex (const xmlpp::Node* node, unsigned int &i);
  void parseStateNode (const xmlpp::Node* node);

  void parseStateActionsNode (const xmlpp::Node* node, State *state);
  void parseStateActionNode (const xmlpp::Node* node, State *state);

  void parseTransitionsNode (const xmlpp::Node* node);
  void parseTransitionNode (const xmlpp::Node* node);

  void parseViewsNode (const xmlpp::Node* node);
  void parseViewNode (const xmlpp::Node* node, const Glib::ustring &plugin, unsigned int &i);
  void parseViewParamsNode (const xmlpp::Node * node, std::list <std::string> &params);
  void parseParamNode (const xmlpp::Node * node, std::list <std::string> &params);
  void parseViewMapNode (const xmlpp::Node* node, View *view);
  
private:
  std::map <Glib::ustring, unsigned int> mStateNameMapper;
  std::map <Glib::ustring, unsigned int> mViewNameMapper;
  std::map <Glib::ustring, Action*> mActionNameMapper;

  Context *mContext;
};
