#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

/* local */
#include "XMLLoader.h"
#include "searchFile.h"
#include "MemoryUtil.h"
#include "stringUtil.h"

/* STD */
#include <cassert>

using namespace std;

static const char *type = "Loader";
static const unsigned int major_version = 1;
static const unsigned int minor_version = 1;

XMLLoader::XMLLoader() :
  mLogger("stateval.plugins.loaders.smxml")
{
}

XMLLoader::~XMLLoader()
{
}

const std::string XMLLoader::getType()
{
  return type;
}

const unsigned int XMLLoader::getMajorVersion()
{
  return major_version;
}

const unsigned int XMLLoader::getMinorVersion()
{
  return minor_version;
}

bool XMLLoader::load(const std::string &sm)
{
  try
  {
    xmlpp::DomParser parser;
    parser.set_validate(true);
    parser.set_substitute_entities();	// We just want the text to be resolved/unescaped automatically.
    parser.parse_file(sm);
    if (parser)
    {
      // Walk the tree:
      const xmlpp::Node *pNode = parser.get_document()->get_root_node();	// deleted by DomParser.
      parseRootNode(pNode);

      // -> delete temporary data maps after constructing statemachine
      //    as the mapper data isn't needed at runtime
      mStateNameMapper.clear();
      mViewNameMapper.clear();
      mActionNameMapper.clear();
      mConditionNameMapper.clear();
      // <-

      return true;
    }
  }
  catch (const exception &ex)
  {
    LOG4CXX_FATAL(mLogger, "Exception caught: " << ex.what());
  }

  return false;
}

void XMLLoader::parseRootNode(const xmlpp::Node *node)
{
  const xmlpp::ContentNode *nodeContent = dynamic_cast < const xmlpp::ContentNode * >(node);
  const xmlpp::TextNode *nodeText = dynamic_cast < const xmlpp::TextNode * >(node);
  const xmlpp::CommentNode *nodeComment = dynamic_cast < const xmlpp::CommentNode * >(node);

  if (nodeText && nodeText->is_white_space())	//Let's ignore the indenting
    return;

  Glib::ustring nodename = node->get_name();

  if (!nodeText && !nodeComment && !nodename.empty())	//Let's not say "name: text".
  {
    if (nodename == "stateval")
    {
      // Recurse through child nodes
      xmlpp::Node::NodeList list = node->get_children();
      for (xmlpp::Node::NodeList::iterator iter = list.begin();
           iter != list.end(); ++iter)
      {
        parseEventsNode(*iter);
      }

      // Recurse through child nodes
      list = node->get_children();
      for (xmlpp::Node::NodeList::iterator iter = list.begin();
           iter != list.end(); ++iter)
      {
        parseVariablesNode(*iter);
      }
      
      // Recurse through child nodes
      list = node->get_children();
      for (xmlpp::Node::NodeList::iterator iter = list.begin();
           iter != list.end(); ++iter)
      {
        parseViewManagerNode(*iter);
      }

      // Recurse through child nodes
      list = node->get_children();
      for (xmlpp::Node::NodeList::iterator iter = list.begin();
           iter != list.end(); ++iter)
      {
        parseConditionsNode(*iter);
      }

      // Recurse through child nodes
      list = node->get_children();
      for (xmlpp::Node::NodeList::iterator iter = list.begin();
           iter != list.end(); ++iter)
      {
        parseActionsNode(*iter);
      }

      // Recurse through child nodes
      list = node->get_children();
      for (xmlpp::Node::NodeList::iterator iter = list.begin();
           iter != list.end(); ++iter)
      {
        parseStatesNode(*iter);
      }

      // Recurse through child nodes
      list = node->get_children();
      for (xmlpp::Node::NodeList::iterator iter = list.begin();
           iter != list.end(); ++iter)
      {
        parseTransitionsNode(*iter);
      }
    }
  }
}

void XMLLoader::parseEventsNode(const xmlpp::Node *node)
{
  const xmlpp::ContentNode *nodeContent = dynamic_cast < const xmlpp::ContentNode * >(node);
  const xmlpp::TextNode *nodeText = dynamic_cast < const xmlpp::TextNode * >(node);
  const xmlpp::CommentNode *nodeComment = dynamic_cast < const xmlpp::CommentNode * >(node);

  if (nodeText && nodeText->is_white_space())	//Let's ignore the indenting
    return;

  Glib::ustring nodename = node->get_name();

  if (!nodeText && !nodeComment && !nodename.empty())	//Let's not say "name: text".
  {
    if (nodename == "events")
    {
      //Recurse through child nodes:
      xmlpp::Node::NodeList list = node->get_children();
      for (xmlpp::Node::NodeList::iterator iter = list.begin();
           iter != list.end(); ++iter)
      {
        parseEventNode(*iter);
      }
    }
  }
}

void XMLLoader::parseEventNode(const xmlpp::Node *node)
{
  const xmlpp::TextNode *nodeText = dynamic_cast < const xmlpp::TextNode * >(node);
  const xmlpp::Element *nodeElement = dynamic_cast < const xmlpp::Element * >(node);

  if (nodeText && nodeText->is_white_space())	//Let's ignore the indenting
    return;

  Glib::ustring nodename = node->get_name();

  if (nodename == "event")
  {
    LOG4CXX_DEBUG(mLogger, "Node = " << node->get_name());

    const xmlpp::Attribute *name_attribute = nodeElement->get_attribute("name");

    if (name_attribute)
    {
      LOG4CXX_DEBUG(mLogger, "Attribute name = " << name_attribute->get_value());

      // add event from XML into statemachine
      addEvent(name_attribute->get_value());
    }
  }
}

void XMLLoader::parseVariablesNode(const xmlpp::Node *node)
{
  const xmlpp::ContentNode *nodeContent = dynamic_cast < const xmlpp::ContentNode * >(node);
  const xmlpp::TextNode *nodeText = dynamic_cast < const xmlpp::TextNode * >(node);
  const xmlpp::CommentNode *nodeComment = dynamic_cast < const xmlpp::CommentNode * >(node);

  if (nodeText && nodeText->is_white_space())	//Let's ignore the indenting
    return;

  Glib::ustring nodename = node->get_name();

  if (!nodeText && !nodeComment && !nodename.empty())	//Let's not say "name: text".
  {
    if (nodename == "variables")
    {
      //Recurse through child nodes:
      xmlpp::Node::NodeList list = node->get_children();
      for (xmlpp::Node::NodeList::iterator iter = list.begin();
           iter != list.end(); ++iter)
      {
        Variable *var = parseVariableNode(*iter);
        if (var)
        {
          const xmlpp::Element *nodeElement = dynamic_cast < const xmlpp::Element * >(*iter);
          const xmlpp::Attribute *name_attribute = nodeElement->get_attribute("name");

          addVariable(name_attribute->get_value(), *var);
        }
      }
    }
  }
}

Variable *XMLLoader::parseVariableNode(const xmlpp::Node *node)
{
  const xmlpp::TextNode *nodeText = dynamic_cast < const xmlpp::TextNode * >(node);
  const xmlpp::Element *nodeElement = dynamic_cast < const xmlpp::Element * >(node);
  Variable *var = NULL;

  if (nodeText && nodeText->is_white_space())	//Let's ignore the indenting
    return NULL;

  Glib::ustring nodename = node->get_name();

  if (nodename == "variable")
  {
    LOG4CXX_DEBUG(mLogger, "Node = " << node->get_name());

    const xmlpp::Attribute *name_attribute = nodeElement->get_attribute("name");
    const xmlpp::Attribute *type_attribute = nodeElement->get_attribute("type");
    const xmlpp::Attribute *value_attribute = nodeElement->get_attribute("value");

    // TODO: error handling

    if (name_attribute)
    {
      LOG4CXX_DEBUG(mLogger, "Attribute name = " << name_attribute->get_value());
    }

    if (type_attribute)
    {
      LOG4CXX_DEBUG(mLogger, "Attribute type = " << type_attribute->get_value());
    }

    if (value_attribute)
    {
      LOG4CXX_DEBUG(mLogger, "Attribute value = " << value_attribute->get_value());
    }

    if (type_attribute->get_value() == "Bool")
    {
      if (value_attribute->get_value() == "true")
      {
        var = new Bool(true);
      }
      else if (value_attribute->get_value() == "false")
      {
        var = new Bool(false);
      }
      else
      {
        // TODO: handle error
        LOG4CXX_ERROR(mLogger, "error: not allowed value");
        assert(false);
      }
    }
    else if (type_attribute->get_value() == "String")
    {
      var = new String(value_attribute->get_value());
    }
    else if (type_attribute->get_value() == "Double")
    {
      var = new Double(fromString <float> (value_attribute->get_value()));
    }
    else if (type_attribute->get_value() == "Integer")
    {
      var = new Integer(fromString <int> (value_attribute->get_value()));
    }
    else if (type_attribute->get_value() == "VoidPtr")
    {
      // always create VoidPtr with NULL from XML
      var = new VoidPtr(NULL);
    }
    else if (type_attribute->get_value() == "Struct")
    {
      Struct *st = new Struct();

      //Recurse through child nodes:
      xmlpp::Node::NodeList list = node->get_children();
      for (xmlpp::Node::NodeList::iterator iter = list.begin();
           iter != list.end(); ++iter)
      {
        Variable *av = parseVariableNode(*iter);
        if (av)
        {
          const xmlpp::Element *innerNodeElement = dynamic_cast < const xmlpp::Element * >(*iter);
          const xmlpp::Attribute *inner_name_attribute = innerNodeElement->get_attribute("name");

          if(inner_name_attribute)
          {
            LOG4CXX_DEBUG(mLogger, "adding variable to struct: " << inner_name_attribute->get_value());
            st->add(inner_name_attribute->get_value(), av);
          }
          else
          {
            LOG4CXX_FATAL(mLogger, "no 'name' attribute specified");
            assert(false);
          }
        }
      }

      var = st;
    }
    else if (type_attribute->get_value() == "List")
    {
      List *ls = new List();

      //Recurse through child nodes:
      xmlpp::Node::NodeList list = node->get_children();
      for (xmlpp::Node::NodeList::iterator iter = list.begin();
           iter != list.end(); ++iter)
      {
        Variable *av = parseVariableNode(*iter);
        if (av)
        {
          ls->pushBack(av);
        }
      }

      var = ls;
    }
    else
    {
      // TODO: handle error
      LOG4CXX_ERROR(mLogger, "error: not allowed type: " << type_attribute->get_value());
      assert(false);
    }
  }

  return var;
}

void XMLLoader::parseConditionsNode(const xmlpp::Node *node)
{
  const xmlpp::ContentNode *nodeContent = dynamic_cast < const xmlpp::ContentNode * >(node);
  const xmlpp::TextNode *nodeText = dynamic_cast < const xmlpp::TextNode * >(node);
  const xmlpp::CommentNode *nodeComment = dynamic_cast < const xmlpp::CommentNode * >(node);

  if (nodeText && nodeText->is_white_space())	//Let's ignore the indenting
    return;

  Glib::ustring nodename = node->get_name();

  if (!nodeText && !nodeComment && !nodename.empty())	//Let's not say "name: text".
  {
    if (nodename == "conditions")
    {
      //Recurse through child nodes:
      xmlpp::Node::NodeList list = node->get_children();
      for (xmlpp::Node::NodeList::iterator iter = list.begin();
           iter != list.end(); ++iter)
      {
        parseConditionNode(*iter);
      }
    }
  }
}

void XMLLoader::parseConditionNode(const xmlpp::Node *node)
{
  const xmlpp::TextNode *nodeText = dynamic_cast < const xmlpp::TextNode * >(node);
  const xmlpp::Element *nodeElement = dynamic_cast < const xmlpp::Element * >(node);

  if (nodeText && nodeText->is_white_space())	//Let's ignore the indenting
    return;

  Glib::ustring nodename = node->get_name();

  if (nodename == "condition")
  {
    LOG4CXX_DEBUG(mLogger, "Node = " << node->get_name());

    const xmlpp::Attribute *name_attribute = nodeElement->get_attribute("name");
    const xmlpp::Attribute *variable_attribute = nodeElement->get_attribute("variable");
    const xmlpp::Attribute *operation_attribute = nodeElement->get_attribute("operation");
    const xmlpp::Attribute *variable2_attribute = nodeElement->get_attribute("variable2");

    if (name_attribute)
    {
      LOG4CXX_DEBUG(mLogger, "Attribute name = " << name_attribute->get_value());
    }

    if (variable_attribute)
    {
      LOG4CXX_DEBUG(mLogger, "Attribute variable = " << variable_attribute->get_value());
    }

    // ignore currently, and use "equals" as default...
    if (operation_attribute)
    {
      LOG4CXX_DEBUG(mLogger, "Attribute operation = " << operation_attribute->get_value());
    }

    if (variable2_attribute)
    {
      LOG4CXX_DEBUG(mLogger, "Attribute operation = " << variable2_attribute->get_value());
    }

    Variable *av = getVariable(variable2_attribute->get_value());
    assert(av);

    Condition *cond = new Condition();
    cond->addComparison(variable_attribute->get_value(), av);

    // temporary save condition pointer for later reference in transition table
    mConditionNameMapper[name_attribute->get_value()] = cond;
  }
}

void XMLLoader::parseActionsNode(const xmlpp::Node *node)
{
  const xmlpp::ContentNode *nodeContent = dynamic_cast < const xmlpp::ContentNode * >(node);
  const xmlpp::TextNode *nodeText = dynamic_cast < const xmlpp::TextNode * >(node);
  const xmlpp::CommentNode *nodeComment = dynamic_cast < const xmlpp::CommentNode * >(node);

  if (nodeText && nodeText->is_white_space())	//Let's ignore the indenting
    return;

  Glib::ustring nodename = node->get_name();

  if (!nodeText && !nodeComment && !nodename.empty())	//Let's not say "name: text".
  {
    if (nodename == "actions")
    {
      //Recurse through child nodes:
      xmlpp::Node::NodeList list = node->get_children();
      for (xmlpp::Node::NodeList::iterator iter = list.begin();
           iter != list.end(); ++iter)
      {
        parseActionNode(*iter);
      }
    }
  }
}

void XMLLoader::parseActionNode(const xmlpp::Node *node)
{
  const xmlpp::TextNode *nodeText = dynamic_cast < const xmlpp::TextNode * >(node);
  const xmlpp::Element *nodeElement = dynamic_cast < const xmlpp::Element * >(node);
  Action *action = NULL;

  if (nodeText && nodeText->is_white_space())	//Let's ignore the indenting
    return;

  Glib::ustring nodename = node->get_name();

  if (nodename == "action")
  {
    LOG4CXX_DEBUG(mLogger, "Node = " << node->get_name());

    const xmlpp::Attribute *name_attribute = nodeElement->get_attribute("name");
    const xmlpp::Attribute *type_attribute = nodeElement->get_attribute("type");
    const xmlpp::Attribute *event_attribute = nodeElement->get_attribute("event");
    const xmlpp::Attribute *variable_attribute = nodeElement->get_attribute("variable");
    const xmlpp::Attribute *widget_attribute = nodeElement->get_attribute("widget");
    const xmlpp::Attribute *property_attribute = nodeElement->get_attribute("property");
    const xmlpp::Attribute *view_attribute = nodeElement->get_attribute("view");

    if (name_attribute)
    {
      LOG4CXX_DEBUG(mLogger, "Attribute name = " << name_attribute->get_value());
    }

    if (event_attribute)
    {
      LOG4CXX_DEBUG(mLogger, "Attribute event = " << event_attribute->get_value());
    }
    if (type_attribute->get_value() == "FireEventAction")
    {
      // TODO: check if event is available/useful
      action = new FireEventAction(findMapingEvent(event_attribute->get_value()));
      mActionNameMapper[name_attribute->get_value()] = action;
    }
    else if (type_attribute->get_value() == "ChangeWidgetPropertyAction")
    {
      if(view_attribute && variable_attribute && widget_attribute && property_attribute)
      {
        const string &view = view_attribute->get_value();
        const string &variable = variable_attribute->get_value();
        const string &widget = widget_attribute->get_value();
        const string &property = property_attribute->get_value();

        // TODO: check if view, widget, variable are really existing before      
        action = new ChangeWidgetPropertyAction(view, widget, property, variable);
        mActionNameMapper[name_attribute->get_value()] = action;
      }
      else
      {
        assert(false);
      }
    }
    else if (type_attribute->get_value() == "ReadWidgetPropertyAction")
    {
      if(view_attribute && variable_attribute && widget_attribute && property_attribute)
      {
        const string &view = view_attribute->get_value();
        const string &variable = variable_attribute->get_value();
        const string &widget = widget_attribute->get_value();
        const string &property = property_attribute->get_value();

        // TODO: check if view, widget, variable are really existing before      
        action = new ReadWidgetPropertyAction(view, widget, property, variable);
        mActionNameMapper[name_attribute->get_value()] = action;
      }
      else
      {
        assert(false);
      }
    }
    else
    {
      cerr << "unknow action" << endl;
      assert(false);
    }

    assert(action);
    addAction(action);
  }
}

void XMLLoader::parseStatesNode(const xmlpp::Node *node)
{
  const xmlpp::ContentNode *nodeContent = dynamic_cast < const xmlpp::ContentNode * >(node);
  const xmlpp::TextNode *nodeText = dynamic_cast < const xmlpp::TextNode * >(node);
  const xmlpp::CommentNode *nodeComment = dynamic_cast < const xmlpp::CommentNode * >(node);

  if (nodeText && nodeText->is_white_space())	//Let's ignore the indenting
    return;

  Glib::ustring nodename = node->get_name();

  if (!nodeText && !nodeComment && !nodename.empty())	//Let's not say "name: text".
  {
    if (nodename == "states")
    {
      // Recurse through child nodes (create Index)
      unsigned int i = 0;
      xmlpp::Node::NodeList list = node->get_children();
      for (xmlpp::Node::NodeList::iterator iter = list.begin();
           iter != list.end();
           ++iter)
      {
        parseStateNodeIndex(*iter, i);
      }

      // Recurse through child nodes
      list = node->get_children();
      for (xmlpp::Node::NodeList::iterator iter = list.begin();
           iter != list.end();
           ++iter)
      {
        parseStateNode(*iter);
      }
    }
  }
}

void XMLLoader::parseStateNodeIndex(const xmlpp::Node *node, unsigned int &i)
{
  const xmlpp::TextNode *nodeText = dynamic_cast < const xmlpp::TextNode * >(node);
  const xmlpp::Element *nodeElement = dynamic_cast < const xmlpp::Element * >(node);

  if (nodeText && nodeText->is_white_space())	//Let's ignore the indenting
    return;

  Glib::ustring nodename = node->get_name();

  if (nodename == "state")
  {
    LOG4CXX_DEBUG(mLogger, "Node (Index) = " << node->get_name());

    const xmlpp::Attribute *name_attribute = nodeElement->get_attribute("name");

    if (name_attribute)
    {
      const Glib::ustring &name = name_attribute->get_value();
      LOG4CXX_DEBUG(mLogger, "Attribute name (Index) = " << name);
      ++i; // modifies also outside of function!
      mStateNameMapper[name] = i;
    }
  }
}

void XMLLoader::parseStateNode(const xmlpp::Node *node)
{
  const xmlpp::TextNode *nodeText = dynamic_cast < const xmlpp::TextNode * >(node);
  const xmlpp::Element *nodeElement = dynamic_cast < const xmlpp::Element * >(node);

  if (nodeText && nodeText->is_white_space()) // Let's ignore the indenting
    return;

  Glib::ustring nodename = node->get_name();

  if (nodename == "state")
  {
    LOG4CXX_DEBUG(mLogger, "Node = " << node->get_name());

    const xmlpp::Attribute *name_attribute = nodeElement->get_attribute("name");
    const xmlpp::Attribute *type_attribute = nodeElement->get_attribute("type");
    const xmlpp::Attribute *parent_attribute = nodeElement->get_attribute("parent");
    const xmlpp::Attribute *view_attribute = nodeElement->get_attribute("view");

    State *state = NULL;
    CompoundState *parentState = NULL;
    int parentNum = 0;
    static bool rootCompoundDefined = false;

    if (name_attribute)
    {
      LOG4CXX_DEBUG(mLogger, "Attribute name = " << name_attribute->get_value());
    }
    else
    {
      // throw exception
    }

    if (parent_attribute)
    {
      const Glib::ustring &parent = parent_attribute->get_value();
      LOG4CXX_DEBUG(mLogger, "Attribute parent = " << parent);

      std::map <Glib::ustring, unsigned int>::iterator snm_it = mStateNameMapper.find(parent);
      if (snm_it != mStateNameMapper.end())
      {
        parentNum = snm_it->second;
      }
      else
      {
        LOG4CXX_FATAL(mLogger, "Couldn't find this parent state in list: " << parent);
        assert (false);
        // TODO: exception?
      }

      parentState = static_cast <CompoundState *>(mStateList[parentNum - 1]);
      assert(parentState);
    }
    else
    {
      // if no parent attribute is defined mark it as root node (only one allowed!)
      parentNum = 0;
    }

    // check type and throw exception
    if (view_attribute)
    {
      LOG4CXX_DEBUG(mLogger, "Attribute view = " << view_attribute->get_value());
    }

    if (type_attribute)
    {
      const Glib::ustring &type = type_attribute->get_value();

      LOG4CXX_DEBUG(mLogger, "Attribute type = " << type);

      if (type == "CompoundState")
      {
        if ((parentNum == 0) && (!rootCompoundDefined)) // detection of root compound
        {
          state = new CompoundState();
          rootCompoundDefined = true;
        }
        else
        {
          state = new CompoundState(parentState);
        }
      }
      else if (type == "SimpleState")
      {
        state = new SimpleState(parentState);
      }
      else if (type == "HistoryState")
      {
        HistoryState *historyState = new HistoryState(parentState);

        parentState->setHistory(historyState);
        state = historyState;
      }
      else if (type == "DecisionState")
      {
        state = new DecisionState(parentState);
      }
      else if (type == "ViewState")
      {
        state = new ViewState(parentState, &mViewCache);
        ViewState *viewState = static_cast <ViewState *>(state);
        assert (viewState);

        if (view_attribute)
        {
          // TODO: better use find() to detect if not found in map
          View *view = mViewNameMapper[view_attribute->get_value()];
          assert (view);
          viewState->addView(*view, 0);
        }
        else // multible view feature
        {
          // Recurse through child nodes
          xmlpp::Node::NodeList list = node->get_children();
          for (xmlpp::Node::NodeList::iterator iter = list.begin();
               iter != list.end(); ++iter)
          {
            parseStateViewsNode(*iter, state);
          }
        }
      }
      else if (type == "InitialState")
      {
        /* InitialState is no real state type. It's mapped on a simple state
         * and just a transition is created from the outer CompoundState to the
         * current state. Only one InitialState for a CompoundState is allowed.
         */

        state = new SimpleState(parentState);

        Transition *trans = new Transition(state);
        parentState->addLeaveTransition(*trans);
        
      }
      else if (type == "FinalState")
      {
        /* FinalState is no real state type. It's mapped on a simple state
         * and just a transition is created from the current state to the outer
         * CompoundState. Only one FinalState for a CompoundState is allowed.
         * If a FinalState in the root CompoundState is waked the statemachine ends.
         */

        state = new SimpleState(parentState);

        Transition *trans = new Transition(parentState);
        state->addLeaveTransition(*trans);
      }
      else
      {
        LOG4CXX_FATAL(mLogger, "Unknown state type: " << type);
        assert (false);
        // throw exception
      }

      assert(state);

      state->setID(mStateNameMapper[name_attribute->get_value()]);
      state->setName(name_attribute->get_value());

      // Recurse through child nodes
      xmlpp::Node::NodeList list = node->get_children();
      for (xmlpp::Node::NodeList::iterator iter = list.begin();
           iter != list.end(); ++iter)
      {
        parseStateActionsNode(*iter, state);
      }

      addState(state);
    }
    else
    {
      // throw Exception
    }
  }
}

void XMLLoader::parseStateViewsNode(const xmlpp::Node *node, State *state)
{
  const xmlpp::ContentNode *nodeContent = dynamic_cast < const xmlpp::ContentNode * >(node);
  const xmlpp::TextNode *nodeText = dynamic_cast < const xmlpp::TextNode * >(node);
  const xmlpp::CommentNode *nodeComment = dynamic_cast < const xmlpp::CommentNode * >(node);
  int viewCounter = 0;

  if (nodeText && nodeText->is_white_space())	//Let's ignore the indenting
    return;

  Glib::ustring nodename = node->get_name();

  if (!nodeText && !nodeComment && !nodename.empty())	//Let's not say "name: text".
  {
    if (nodename == "views")
    {
      //Recurse through child nodes:
      xmlpp::Node::NodeList list = node->get_children();
      for (xmlpp::Node::NodeList::iterator iter = list.begin();
           iter != list.end(); ++iter)
      {
        parseStateViewNode(*iter, state, viewCounter);
      }
    }
  }
}

void XMLLoader::parseStateViewNode(const xmlpp::Node *node, State *state, int &viewCounterOut)
{
  const xmlpp::TextNode *nodeText = dynamic_cast < const xmlpp::TextNode * >(node);
  const xmlpp::Element *nodeElement = dynamic_cast < const xmlpp::Element * >(node);

  if (nodeText && nodeText->is_white_space())	//Let's ignore the indenting
    return;

  Glib::ustring nodename = node->get_name();

  if (nodename == "view")
  {
    LOG4CXX_DEBUG(mLogger, "Node = " << node->get_name());

    const xmlpp::Attribute *ref_attribute = nodeElement->get_attribute("ref");

    if (ref_attribute)
    {
      LOG4CXX_DEBUG(mLogger, "Attribute ref = " << ref_attribute->get_value());
    }

    // TODO: use find...
    View *view = mViewNameMapper[ref_attribute->get_value()];
    assert (view);
    ViewState *viewState = static_cast <ViewState *>(state);
    assert (viewState);
    viewState->addView(*view, viewCounterOut);

    ++viewCounterOut;
  }
}

void XMLLoader::parseStateActionsNode(const xmlpp::Node *node, State *state)
{
  const xmlpp::ContentNode *nodeContent = dynamic_cast < const xmlpp::ContentNode * >(node);
  const xmlpp::TextNode *nodeText = dynamic_cast < const xmlpp::TextNode * >(node);
  const xmlpp::CommentNode *nodeComment = dynamic_cast < const xmlpp::CommentNode * >(node);

  if (nodeText && nodeText->is_white_space())	//Let's ignore the indenting
    return;

  Glib::ustring nodename = node->get_name();

  if (!nodeText && !nodeComment && !nodename.empty())	//Let's not say "name: text".
  {
    if (nodename == "actions")
    {
      //Recurse through child nodes:
      xmlpp::Node::NodeList list = node->get_children();
      for (xmlpp::Node::NodeList::iterator iter = list.begin();
           iter != list.end(); ++iter)
      {
        parseStateActionNode(*iter, state);
      }
    }
  }
}

void XMLLoader::parseStateActionNode(const xmlpp::Node *node, State *state)
{
  const xmlpp::TextNode *nodeText = dynamic_cast < const xmlpp::TextNode * >(node);
  const xmlpp::Element *nodeElement = dynamic_cast < const xmlpp::Element * >(node);

  if (nodeText && nodeText->is_white_space())	//Let's ignore the indenting
    return;

  Glib::ustring nodename = node->get_name();

  if (nodename == "action")
  {
    LOG4CXX_DEBUG(mLogger, "Node = " << node->get_name());

    const xmlpp::Attribute *ref_attribute = nodeElement->get_attribute("ref");
    const xmlpp::Attribute *when_attribute = nodeElement->get_attribute("when");

    if (ref_attribute)
    {
      LOG4CXX_DEBUG(mLogger, "Attribute ref = " << ref_attribute->get_value());
    }

    if (when_attribute)
    {
      LOG4CXX_DEBUG(mLogger, "Attribute when = " << when_attribute->get_value());
    }

    // TODO: use find...
    Action *action = mActionNameMapper[ref_attribute->get_value()];
    assert(action);

    if (when_attribute->get_value() == "enter")
    {
      state->addEntryAction(action);
    }
    else if (when_attribute->get_value() == "leave")
    {
      state->addExitAction(action);
    }
    else
    {
      assert(false);
    }
  }
}

void XMLLoader::parseTransitionsNode(const xmlpp::Node *node)
{
  const xmlpp::ContentNode *nodeContent = dynamic_cast < const xmlpp::ContentNode * >(node);
  const xmlpp::TextNode *nodeText = dynamic_cast < const xmlpp::TextNode * >(node);
  const xmlpp::CommentNode *nodeComment = dynamic_cast < const xmlpp::CommentNode * >(node);

  if (nodeText && nodeText->is_white_space())	//Let's ignore the indenting
    return;

  Glib::ustring nodename = node->get_name();

  if (!nodeText && !nodeComment && !nodename.empty())	//Let's not say "name: text".
  {
    if (nodename == "transitions")
    {
      //Recurse through child nodes:
      xmlpp::Node::NodeList list = node->get_children();
      for (xmlpp::Node::NodeList::iterator iter = list.begin();
           iter != list.end(); ++iter)
      {
        parseTransitionNode(*iter);
      }
    }
  }
}

void XMLLoader::parseTransitionNode(const xmlpp::Node *node)
{
  const xmlpp::TextNode *nodeText = dynamic_cast < const xmlpp::TextNode * >(node);
  const xmlpp::Element *nodeElement = dynamic_cast < const xmlpp::Element * >(node);

  if (nodeText && nodeText->is_white_space())	//Let's ignore the indenting
    return;

  Glib::ustring nodename = node->get_name();

  if (nodename == "transition")
  {
    LOG4CXX_DEBUG(mLogger, "Node = " << node->get_name());

    const xmlpp::Attribute *from_attribute = nodeElement->get_attribute("from");
    const xmlpp::Attribute *to_attribute = nodeElement->get_attribute("to");
    const xmlpp::Attribute *event_attribute = nodeElement->get_attribute("event");
    const xmlpp::Attribute *decision_attribute = nodeElement->get_attribute("decision");

    int fromStateNum = 0;
    int toStateNum = 0;
    State *fromState = NULL;
    State *toState = NULL;
    Transition *trans = NULL;

    if (from_attribute)
    {
      LOG4CXX_DEBUG(mLogger, "Attribute from = " << from_attribute->get_value());

      std::map<Glib::ustring, unsigned int>::iterator find_it;
      find_it = mStateNameMapper.find(from_attribute->get_value());
      if (find_it != mStateNameMapper.end())
      {
        fromStateNum = find_it->second;
        fromState = mStateList[fromStateNum - 1];
      }
      else
      {
        LOG4CXX_FATAL(mLogger, "Attribute 'from' wrong");
        assert(false);
      }
    }
    else
    {
      LOG4CXX_FATAL(mLogger, "Attribute 'from' undefined");
      assert(false);
    }

    if (to_attribute)
    {
      LOG4CXX_DEBUG(mLogger, "Attribute to = " << to_attribute->get_value());

      std::map<Glib::ustring, unsigned int>::iterator find_it;
      find_it = mStateNameMapper.find(to_attribute->get_value());
      if (find_it != mStateNameMapper.end())
      {
        toStateNum = find_it->second;
        toState = mStateList[toStateNum - 1];
      }
      else
      {
        LOG4CXX_FATAL(mLogger, "Attribute 'to' wrong");
        assert(false);
      }
    }
    else
    {
      LOG4CXX_FATAL(mLogger, "Attribute 'to' undefined");
      assert(false);
    }

    if (event_attribute)
    {
      LOG4CXX_DEBUG(mLogger, "Attribute event = " << event_attribute->get_value());
      trans = new Transition(toState, findMapingEvent(event_attribute->get_value()));
    }
    else
    {
      trans = new Transition(toState);
    }

    if (decision_attribute)
    {
      // TODO: maybe there's a better idea to solve this with correct polymorphy design...
      DecisionState *decisionState = static_cast <DecisionState *>(fromState);

      LOG4CXX_DEBUG(mLogger, "Attribute decision = " << decision_attribute->get_value());
      Condition *cond = mConditionNameMapper[decision_attribute->get_value()];
      std::pair <Condition *, Transition *> conTrans(cond, trans);
      decisionState->addConditionTransition(conTrans);
    }
    else
    {
      fromState->addLeaveTransition(*trans);
    }
  }
}

void XMLLoader::parseViewManagerNode(const xmlpp::Node *node)
{
  const xmlpp::ContentNode *nodeContent = dynamic_cast < const xmlpp::ContentNode * >(node);
  const xmlpp::TextNode *nodeText = dynamic_cast < const xmlpp::TextNode * >(node);
  const xmlpp::CommentNode *nodeComment = dynamic_cast < const xmlpp::CommentNode * >(node);
  const xmlpp::Element *nodeElement = dynamic_cast < const xmlpp::Element * >(node);

  if (nodeText && nodeText->is_white_space())	//Let's ignore the indenting
    return;

  Glib::ustring nodename = node->get_name();

  if (!nodeText && !nodeComment && !nodename.empty())	//Let's not say "name: text".
  {
    if (nodename == "viewmanager")
    {
      const xmlpp::Attribute *plugin_attribute = nodeElement->get_attribute("plugin");

      // Recurse through child nodes BEFORE creating ViewManager
      xmlpp::Node::NodeList list = node->get_children();
      for (xmlpp::Node::NodeList::iterator iter = list.begin();
           iter != list.end(); ++iter)
      {
        parseViewManagerParamsNode(*iter, mViewManagerParams);
      }
      
      if (plugin_attribute)
      {        
        LOG4CXX_DEBUG(mLogger, "Attribute plugin = " << plugin_attribute->get_value());
        int size = mViewManagerParams.size ();
        cout << size << endl;
        loadViewManager (plugin_attribute->get_value(), mViewManagerParams);     
        mViewManager->setDataLoadDir(mDataLoadDir);
      }
      else
      {
        assert (false);
        // throw exception
      }
      
      // Recurse through child nodes
      unsigned int i = 0;
      list = node->get_children();
      for (xmlpp::Node::NodeList::iterator iter = list.begin();
           iter != list.end(); ++iter)
      {
        parseViewNode(*iter, i);
      }


    }
  }
}

void XMLLoader::parseViewManagerParamsNode(const xmlpp::Node *node, std::map <std::string, std::string> &params)
{
  const xmlpp::ContentNode *nodeContent = dynamic_cast < const xmlpp::ContentNode * >(node);
  const xmlpp::TextNode *nodeText = dynamic_cast < const xmlpp::TextNode * >(node);
  const xmlpp::CommentNode *nodeComment = dynamic_cast < const xmlpp::CommentNode * >(node);

  if (nodeText && nodeText->is_white_space())	//Let's ignore the indenting
    return;

  Glib::ustring nodename = node->get_name();

  if (!nodeText && !nodeComment && !nodename.empty())	//Let's not say "name: text".
  {
    if (nodename == "params")
    {
      // Recurse through child nodes
      xmlpp::Node::NodeList list = node->get_children();
      for (xmlpp::Node::NodeList::iterator iter = list.begin();
           iter != list.end(); ++iter)
      {
        parseViewManagerParamNode(*iter, params);
      }
    }
  }
}

void XMLLoader::parseViewManagerParamNode(const xmlpp::Node *node, std::map <std::string, std::string> &params)
{
  const xmlpp::TextNode *nodeText = dynamic_cast < const xmlpp::TextNode * >(node);
  const xmlpp::Element *nodeElement = dynamic_cast < const xmlpp::Element * >(node);

  if (nodeText && nodeText->is_white_space())	//Let's ignore the indenting
    return;

  Glib::ustring nodename = node->get_name();

  if (!nodename.empty())
  {
    LOG4CXX_DEBUG(mLogger, "parseViewManagerParamNode = " << node->get_name());

    const xmlpp::Attribute *key_attribute = nodeElement->get_attribute("key");
    const xmlpp::Attribute *value_attribute = nodeElement->get_attribute("value");

    if (key_attribute && value_attribute)
    {
      LOG4CXX_DEBUG(mLogger, "Attribute key = " << key_attribute->get_value());
      LOG4CXX_DEBUG(mLogger, "Attribute value = " << value_attribute->get_value());

      params[key_attribute->get_value()] = value_attribute->get_value();
    }
  }
}

void XMLLoader::parseViewNode(const xmlpp::Node *node, unsigned int &i)
{
  const xmlpp::ContentNode *nodeContent = dynamic_cast < const xmlpp::ContentNode * >(node);
  const xmlpp::TextNode *nodeText = dynamic_cast < const xmlpp::TextNode * >(node);
  const xmlpp::CommentNode *nodeComment = dynamic_cast < const xmlpp::CommentNode * >(node);
  const xmlpp::Element *nodeElement = dynamic_cast < const xmlpp::Element * >(node);
  View *view = NULL;

  if (nodeText && nodeText->is_white_space())	// Let's ignore the indenting
    return;

  Glib::ustring nodename = node->get_name();

  if (!nodeText && !nodeComment && !nodename.empty())	// Let's not say "name: text".
  {
    if (nodename == "view")
    {
      const xmlpp::Attribute *name_attribute = nodeElement->get_attribute("name");

      if (name_attribute)
      {
        LOG4CXX_DEBUG(mLogger, "Attribute name = " << name_attribute->get_value());
        ++i; // modifies ref variable also outside of function!
      }
      else
      {
        assert (false);
        // throw exception
      }

      map <std::string, std::string> params;

      // Recurse through child nodes
      xmlpp::Node::NodeList list = node->get_children();
      for (xmlpp::Node::NodeList::iterator iter = list.begin();
           iter != list.end(); ++iter)
      {
        parseViewParamsNode(*iter, params);
      }

      // load view and insert into temporary load mapper
      view = mViewManager->loadView(name_attribute->get_value(), params);
      assert (view);
      mViewNameMapper[name_attribute->get_value()] = view;

      // Recurse through child nodes
      list = node->get_children();
      for (xmlpp::Node::NodeList::iterator iter = list.begin();
           iter != list.end(); ++iter)
      {
        parseViewMappingsNode(*iter, view);
      }

      // Recurse through child nodes
      list = node->get_children();
      for (xmlpp::Node::NodeList::iterator iter = list.begin();
           iter != list.end(); ++iter)
      {
        parseViewWidgetsNode(*iter, view);
      }
    }
  }
}

void XMLLoader::parseViewParamsNode(const xmlpp::Node *node, std::map <std::string, std::string> &params)
{
  const xmlpp::ContentNode *nodeContent = dynamic_cast < const xmlpp::ContentNode * >(node);
  const xmlpp::TextNode *nodeText = dynamic_cast < const xmlpp::TextNode * >(node);
  const xmlpp::CommentNode *nodeComment = dynamic_cast < const xmlpp::CommentNode * >(node);

  if (nodeText && nodeText->is_white_space())	//Let's ignore the indenting
    return;

  Glib::ustring nodename = node->get_name();

  if (!nodeText && !nodeComment && !nodename.empty())	//Let's not say "name: text".
  {
    if (nodename == "params")
    {
      // Recurse through child nodes
      xmlpp::Node::NodeList list = node->get_children();
      for (xmlpp::Node::NodeList::iterator iter = list.begin();
           iter != list.end(); ++iter)
      {
        parseViewParamNode(*iter, params);
      }
    }
  }
}

void XMLLoader::parseViewParamNode(const xmlpp::Node *node, std::map <std::string, std::string> &params)
{
  const xmlpp::TextNode *nodeText = dynamic_cast < const xmlpp::TextNode * >(node);
  const xmlpp::Element *nodeElement = dynamic_cast < const xmlpp::Element * >(node);

  if (nodeText && nodeText->is_white_space())	//Let's ignore the indenting
    return;

  Glib::ustring nodename = node->get_name();

  if (!nodename.empty())
  {
    LOG4CXX_DEBUG(mLogger, "Node = " << node->get_name());

    const xmlpp::Attribute *key_attribute = nodeElement->get_attribute("key");
    const xmlpp::Attribute *value_attribute = nodeElement->get_attribute("value");

    if (key_attribute && value_attribute)
    {
      LOG4CXX_DEBUG(mLogger, "Attribute key = " << key_attribute->get_value());
      LOG4CXX_DEBUG(mLogger, "Attribute value = " << value_attribute->get_value());

      params[key_attribute->get_value()] = value_attribute->get_value();
    }
  }
}

void XMLLoader::parseViewMappingsNode(const xmlpp::Node *node, View *view)
{
  const xmlpp::ContentNode *nodeContent = dynamic_cast < const xmlpp::ContentNode * >(node);
  const xmlpp::TextNode *nodeText = dynamic_cast < const xmlpp::TextNode * >(node);
  const xmlpp::CommentNode *nodeComment = dynamic_cast < const xmlpp::CommentNode * >(node);

  if (nodeText && nodeText->is_white_space())	//Let's ignore the indenting
    return;

  Glib::ustring nodename = node->get_name();

  if (!nodeText && !nodeComment && !nodename.empty())	//Let's not say "name: text".
  {
    if (nodename == "mappings")
    {
      // Recurse through child nodes
      xmlpp::Node::NodeList list = node->get_children();
      for (xmlpp::Node::NodeList::iterator iter = list.begin();
           iter != list.end(); ++iter)
      {
        parseViewMapNode(*iter, view);
      }
    }
  }
}

void XMLLoader::parseViewMapNode(const xmlpp::Node *node, View *view)
{
  const xmlpp::TextNode *nodeText = dynamic_cast < const xmlpp::TextNode * >(node);
  const xmlpp::Element *nodeElement = dynamic_cast < const xmlpp::Element * >(node);

  if (nodeText && nodeText->is_white_space())	//Let's ignore the indenting
    return;

  Glib::ustring nodename = node->get_name();

  if (nodename == "map")
  {
    LOG4CXX_DEBUG(mLogger, "Node = " << node->get_name());

    const xmlpp::Attribute *from_attribute = nodeElement->get_attribute("from");
    const xmlpp::Attribute *to_attribute = nodeElement->get_attribute("to");
    int from_event = -1;
    int to_event = -1;
    

    if (from_attribute)
    {
      from_event = findMapingEvent(from_attribute->get_value());
      LOG4CXX_DEBUG(mLogger, "Attribute from = " << from_attribute->get_value() << " => " << from_event);
      
      if(from_event == StateMachine::EMPTY_EVENT)
      {
        assert(false);
      }
    }
    else
    {
      // error handling
      assert(false);
    }

    if (to_attribute)
    {
      to_event = findMapingEvent(to_attribute->get_value());
      LOG4CXX_DEBUG(mLogger, "Attribute to = " << to_attribute->get_value() << " => " << to_event);
      
      if(to_event == StateMachine::EMPTY_EVENT)
      {
        assert(false);
      }
    }
    else
    {
      // error handling
      assert(false);
    }

    if (view)
    {
      view->addEventMapping(from_event, to_event);
    }
    else
    {
      assert (false);
    }
  }
}

void XMLLoader::parseViewWidgetsNode(const xmlpp::Node *node, View *view)
{
  const xmlpp::ContentNode *nodeContent = dynamic_cast < const xmlpp::ContentNode * >(node);
  const xmlpp::TextNode *nodeText = dynamic_cast < const xmlpp::TextNode * >(node);
  const xmlpp::CommentNode *nodeComment = dynamic_cast < const xmlpp::CommentNode * >(node);

  if (nodeText && nodeText->is_white_space())	//Let's ignore the indenting
    return;

  Glib::ustring nodename = node->get_name();

  if (!nodeText && !nodeComment && !nodename.empty())	//Let's not say "name: text".
  {
    if (nodename == "widgets")
    {
      // Recurse through child nodes
      xmlpp::Node::NodeList list = node->get_children();
      for (xmlpp::Node::NodeList::iterator iter = list.begin();
           iter != list.end(); ++iter)
      {
        parseViewWidgetNode(*iter, view);
      }
    }
  }
}

void XMLLoader::parseViewWidgetNode(const xmlpp::Node *node, View *view)
{
  const xmlpp::TextNode *nodeText = dynamic_cast < const xmlpp::TextNode * >(node);
  const xmlpp::Element *nodeElement = dynamic_cast < const xmlpp::Element * >(node);

  if (nodeText && nodeText->is_white_space())	//Let's ignore the indenting
    return;

  Glib::ustring nodename = node->get_name();

  if (nodename == "widget")
  {
    LOG4CXX_DEBUG(mLogger, "Node = " << node->get_name());

    const xmlpp::Attribute *name_attribute = nodeElement->get_attribute("name");
    //const xmlpp::Attribute *variable_attribute = nodeElement->get_attribute("variable");

    if (name_attribute)
    {
      LOG4CXX_DEBUG(mLogger, "Attribute name = " << name_attribute->get_value());
    }
    else
    {
      // throw exception
      assert(false);
    }
    
    Widget *widget = view->createWidget(name_attribute->get_value());

    // Recurse through child nodes
    xmlpp::Node::NodeList list = node->get_children();
    for (xmlpp::Node::NodeList::iterator iter = list.begin();
         iter != list.end(); ++iter)
    {
      parseViewWidgetPropertyNode(*iter, widget);
    }
  }
}

void XMLLoader::parseViewWidgetPropertyNode(const xmlpp::Node *node, Widget *widget)
{
  const xmlpp::TextNode *nodeText = dynamic_cast < const xmlpp::TextNode * >(node);
  const xmlpp::Element *nodeElement = dynamic_cast < const xmlpp::Element * >(node);

  if (nodeText && nodeText->is_white_space())	//Let's ignore the indenting
    return;

  Glib::ustring nodename = node->get_name();

  if (nodename == "property")
  {
    LOG4CXX_DEBUG(mLogger, "Node = " << node->get_name());

    const xmlpp::Attribute *name_attribute = nodeElement->get_attribute("name");
    const xmlpp::Attribute *variable_attribute = nodeElement->get_attribute("variable");

    if (name_attribute && variable_attribute)
    {
      LOG4CXX_DEBUG(mLogger, "Attribute name = " << name_attribute->get_value());
      LOG4CXX_DEBUG(mLogger, "Attribute variable = " << variable_attribute->get_value());

      Variable *value = getVariable(variable_attribute->get_value());
      widget->setProperty(name_attribute->get_value(), *value);
    }
    else
    {
      // throw exception
      assert(false);
    }

  }
}

/*****************************/
/* Plugin needed C functions */
/*****************************/

PLUGIN_EXPORT XMLLoader *plugin_create()
{
  return new XMLLoader;
}

PLUGIN_EXPORT void plugin_destroy(Loader *plugin)
{
  delete plugin;
}

PLUGIN_EXPORT const char *get_plugin_type()
{
  return type;
}

PLUGIN_EXPORT unsigned int get_plugin_major_version()
{
  return major_version;
}
