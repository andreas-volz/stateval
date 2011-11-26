#ifndef XML_LOADER_H
#define XML_LOADER_H

/* STD */
#include <iostream>
#include <string>

/* XML */
#include <libxml++/libxml++.h>

/* stateval */
#include "stateval/stateval.h"
#include "stateval/private/stateval_private.h"

/* local */
#include "Logger.h"

class XMLLoader : public Loader
{
public:
  XMLLoader();
  ~XMLLoader();

  const std::string getType();

  const unsigned int getMajorVersion();

  const unsigned int getMinorVersion();

  bool load(Context *context, const std::string &sm);

protected:
  void parseRootNode(const xmlpp::Node *node);

  void parseEventsNode(const xmlpp::Node *node);
  void parseEventNode(const xmlpp::Node *node);

  void parseVariablesNode(const xmlpp::Node *node);
  AbstractVariable *parseVariableNode(const xmlpp::Node *node);

  void parseConditionsNode(const xmlpp::Node *node);
  void parseConditionNode(const xmlpp::Node *node);

  void parseActionsNode(const xmlpp::Node *node);
  void parseActionNode(const xmlpp::Node *node);

  void parseStatesNode(const xmlpp::Node *node);
  void parseStateNodeIndex(const xmlpp::Node *node, unsigned int &i);
  void parseStateNode(const xmlpp::Node *node);

  void parseStateViewsNode(const xmlpp::Node *node, State *state);
  void parseStateViewNode(const xmlpp::Node *node, State *state, int &viewCounterOut);

  void parseStateActionsNode(const xmlpp::Node *node, State *state);
  void parseStateActionNode(const xmlpp::Node *node, State *state);

  void parseTransitionsNode(const xmlpp::Node *node);
  void parseTransitionNode(const xmlpp::Node *node);

  void parseViewsNode(const xmlpp::Node *node);
  void parseViewNode(const xmlpp::Node *node, const Glib::ustring &plugin, unsigned int &i);

  void parseViewParamsNode(const xmlpp::Node *node, std::list <std::string> &params);
  void parseViewParamNode(const xmlpp::Node *node, std::list <std::string> &params);

  void parseViewMappingsNode(const xmlpp::Node *node, View *view);
  void parseViewMapNode(const xmlpp::Node *node, View *view);

  void parseViewWidgetsNode(const xmlpp::Node *node, View *view);
  void parseViewWidgetNode(const xmlpp::Node *node, View *view);

private:
  Logger mLogger;

  // -> these maps are only temporary needed while loading, after this they're freed!
  std::map <Glib::ustring, unsigned int> mStateNameMapper;
  std::map <Glib::ustring, unsigned int> mViewNameMapper;
  std::map <Glib::ustring, Action *> mActionNameMapper;
  std::map <Glib::ustring, Condition *> mConditionNameMapper;
  // <-

  Context *mContext;
};

#endif // XML_LOADER_H