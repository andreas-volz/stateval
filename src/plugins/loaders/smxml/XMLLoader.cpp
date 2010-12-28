#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

/* local */
#include "XMLLoader.h"
#include "searchFile.h"
#include "localUtil.h"

/* STD */
#include <cassert>

using namespace std;

static const char* type = "Loader";
static const unsigned int major_version = 1;
static const unsigned int minor_version = 1;

XMLLoader::XMLLoader () :
  mContext (NULL)
{

}

XMLLoader::~XMLLoader ()
{
  unload ();
}

const std::string XMLLoader::getType ()
{
  return type;
}

const unsigned int XMLLoader::getMajorVersion ()
{
  return major_version;
}

const unsigned int XMLLoader::getMinorVersion ()
{
  return minor_version;
}

bool XMLLoader::load (Context *context, const std::string &sm)
{
  mContext = context;
  
  try
  {
    xmlpp::DomParser parser;
    parser.set_validate (false);	// TODO: activate DTD later...
    parser.set_substitute_entities ();	//We just want the text to be resolved/unescaped automatically.
    parser.parse_file (sm);
    if (parser)
    {
      //Walk the tree:
      const xmlpp::Node * pNode = parser.get_document ()->get_root_node ();	//deleted by DomParser.
      parseRootNode (pNode);

     // delete temporary data maps after constructing statemachine
     // as the mapper data isn't needed at runtime
     mStateNameMapper.clear ();
     mViewNameMapper.clear ();
     mActionNameMapper.clear ();
     mConditionNameMapper.clear ();
    }
  }
  catch (const exception &ex)
  {
    std::cout << "Exception caught: " << ex.what () << std::endl;
  }
}

void XMLLoader::unload ()
{
  // free mViewList
  // TODO: as View is a plugin it should not be deleted like here!
  delete_stl_container <std::vector <View*>, View*> (mViewList);

  // free mStateList
  delete_stl_container <std::vector <State*>, State*> (mStateList);

  // free mActionList
  delete_stl_container <std::list <Action*>, Action*> (mActionList);
}

void XMLLoader::parseRootNode (const xmlpp::Node * node)
{
  const xmlpp::ContentNode * nodeContent = dynamic_cast < const xmlpp::ContentNode * >(node);
  const xmlpp::TextNode * nodeText = dynamic_cast < const xmlpp::TextNode * >(node);
  const xmlpp::CommentNode * nodeComment = dynamic_cast < const xmlpp::CommentNode * >(node);

  if (nodeText && nodeText->is_white_space ())	//Let's ignore the indenting
    return;

  Glib::ustring nodename = node->get_name ();

  if (!nodeText && !nodeComment && !nodename.empty ())	//Let's not say "name: text".
  {
    if (nodename == "stateval")
    {
      // Recurse through child nodes
      xmlpp::Node::NodeList list = node->get_children ();
      for (xmlpp::Node::NodeList::iterator iter = list.begin ();
           iter != list.end (); ++iter)
      {
        parseEventsNode (*iter);
      }

      // Recurse through child nodes
      list = node->get_children ();
      for (xmlpp::Node::NodeList::iterator iter = list.begin ();
           iter != list.end (); ++iter)
      {
        parseTypesNode (*iter);
      }
      
      // Recurse through child nodes
      list = node->get_children ();
      for (xmlpp::Node::NodeList::iterator iter = list.begin ();
           iter != list.end (); ++iter)
      {
        parseVariablesNode (*iter);
      }

      // Recurse through child nodes
      list = node->get_children ();
      for (xmlpp::Node::NodeList::iterator iter = list.begin ();
           iter != list.end (); ++iter)
      {
        parseConditionsNode (*iter);
      }
      
      // Recurse through child nodes
      list = node->get_children ();
      for (xmlpp::Node::NodeList::iterator iter = list.begin ();
           iter != list.end (); ++iter)
      {
        parseActionsNode (*iter);
      }
      
      // Recurse through child nodes
      list = node->get_children ();
      for (xmlpp::Node::NodeList::iterator iter = list.begin ();
           iter != list.end (); ++iter)
      {
        parseViewsNode (*iter);
      }

      // Recurse through child nodes
      list = node->get_children ();
      for (xmlpp::Node::NodeList::iterator iter = list.begin ();
           iter != list.end (); ++iter)
      {
        parseStatesNode (*iter);
      }

      // Recurse through child nodes
      list = node->get_children ();
      for (xmlpp::Node::NodeList::iterator iter = list.begin ();
           iter != list.end (); ++iter)
      {
        parseTransitionsNode (*iter);
      }
    }
  }
}

void XMLLoader::parseEventsNode (const xmlpp::Node * node)
{
  const xmlpp::ContentNode * nodeContent = dynamic_cast < const xmlpp::ContentNode * >(node);
  const xmlpp::TextNode * nodeText = dynamic_cast < const xmlpp::TextNode * >(node);
  const xmlpp::CommentNode * nodeComment = dynamic_cast < const xmlpp::CommentNode * >(node);

  if (nodeText && nodeText->is_white_space ())	//Let's ignore the indenting
    return;

  Glib::ustring nodename = node->get_name ();

  if (!nodeText && !nodeComment && !nodename.empty ())	//Let's not say "name: text".
  {
    if (nodename == "events")
    {
      //Recurse through child nodes:
      xmlpp::Node::NodeList list = node->get_children ();
      for (xmlpp::Node::NodeList::iterator iter = list.begin ();
           iter != list.end (); ++iter)
      {
        parseEventNode (*iter);
      }
    }
  }
}

void XMLLoader::parseEventNode (const xmlpp::Node * node)
{
  const xmlpp::TextNode * nodeText = dynamic_cast < const xmlpp::TextNode * >(node);
  const xmlpp::Element * nodeElement = dynamic_cast < const xmlpp::Element * >(node);
  
  if (nodeText && nodeText->is_white_space ())	//Let's ignore the indenting
    return;

  Glib::ustring nodename = node->get_name ();

  if (nodename == "event")
  {
    cout << "Node = " << node->get_name () << endl;

    const xmlpp::Attribute *name_attribute = nodeElement->get_attribute ("name");

    if (name_attribute)
    {
      cout << "Attribute name = " << name_attribute->get_value () << endl;

      // add event from XML into statemachine
      addEvent (name_attribute->get_value ());
    }
  }
}

void XMLLoader::parseVariablesNode (const xmlpp::Node * node)
{
  const xmlpp::ContentNode * nodeContent = dynamic_cast < const xmlpp::ContentNode * >(node);
  const xmlpp::TextNode * nodeText = dynamic_cast < const xmlpp::TextNode * >(node);
  const xmlpp::CommentNode * nodeComment = dynamic_cast < const xmlpp::CommentNode * >(node);

  if (nodeText && nodeText->is_white_space ())	//Let's ignore the indenting
    return;

  Glib::ustring nodename = node->get_name ();

  if (!nodeText && !nodeComment && !nodename.empty ())	//Let's not say "name: text".
  {
    if (nodename == "variables")
    {
      //Recurse through child nodes:
      xmlpp::Node::NodeList list = node->get_children ();
      for (xmlpp::Node::NodeList::iterator iter = list.begin ();
           iter != list.end (); ++iter)
      {
        parseVariableNode (*iter);
      }
    }
  }
}

void XMLLoader::parseVariableNode (const xmlpp::Node * node)
{
  const xmlpp::TextNode * nodeText = dynamic_cast < const xmlpp::TextNode * >(node);
  const xmlpp::Element * nodeElement = dynamic_cast < const xmlpp::Element * >(node);
  
  if (nodeText && nodeText->is_white_space ())	//Let's ignore the indenting
    return;

  Glib::ustring nodename = node->get_name ();

  if (nodename == "variable")
  {
    cout << "Node = " << node->get_name () << endl;

    const xmlpp::Attribute *name_attribute = nodeElement->get_attribute ("name");
    const xmlpp::Attribute *type_attribute = nodeElement->get_attribute ("type");
    const xmlpp::Attribute *value_attribute = nodeElement->get_attribute ("value");

    // TODO: error handling
    
    if (name_attribute)
    {
      cout << "Attribute name = " << name_attribute->get_value () << endl;
    }

    if (type_attribute)
    {
      cout << "Attribute type = " << type_attribute->get_value () << endl;
    }


    if (value_attribute)
    {
      cout << "Attribute value = " << value_attribute->get_value () << endl;
    }

    AbstractVariable *var = NULL;

    // TODO: create helper function and use together with parseConditionVariableNode ()
    if (type_attribute->get_value () == "Bool")
    {
      if (value_attribute->get_value () == "true")
      {
        var = new Bool (true);
      }
      else if (value_attribute->get_value () == "false")
      {
        var = new Bool (false);
      }
      else
      {
        // TODO: handle error
        cerr << "error: not allowed value" << endl;
        assert (false);
      }
    }
    else if (type_attribute->get_value () == "String")
    {
      var = new String (value_attribute->get_value ());
    }
    else
    {
      // TODO: handle error
      cerr << "error: not allowed type" << endl;
      assert (false);
    }
    
    GlobalVariables &global = GlobalVariables::instance ();
    global.addVariable (name_attribute->get_value (), *var);
    
  }
}

void XMLLoader::parseConditionsNode (const xmlpp::Node * node)
{
  const xmlpp::ContentNode * nodeContent = dynamic_cast < const xmlpp::ContentNode * >(node);
  const xmlpp::TextNode * nodeText = dynamic_cast < const xmlpp::TextNode * >(node);
  const xmlpp::CommentNode * nodeComment = dynamic_cast < const xmlpp::CommentNode * >(node);

  if (nodeText && nodeText->is_white_space ())	//Let's ignore the indenting
    return;

  Glib::ustring nodename = node->get_name ();

  if (!nodeText && !nodeComment && !nodename.empty ())	//Let's not say "name: text".
  {
    if (nodename == "conditions")
    {
      //Recurse through child nodes:
      xmlpp::Node::NodeList list = node->get_children ();
      for (xmlpp::Node::NodeList::iterator iter = list.begin ();
           iter != list.end (); ++iter)
      {
        parseConditionNode (*iter);
      }
    }
  }
}

void XMLLoader::parseConditionNode (const xmlpp::Node * node)
{
  const xmlpp::TextNode * nodeText = dynamic_cast < const xmlpp::TextNode * >(node);
  const xmlpp::Element * nodeElement = dynamic_cast < const xmlpp::Element * >(node);
  
  if (nodeText && nodeText->is_white_space ())	//Let's ignore the indenting
    return;

  Glib::ustring nodename = node->get_name ();

  if (nodename == "condition")
  {
    cout << "Node = " << node->get_name () << endl;

    const xmlpp::Attribute *name_attribute = nodeElement->get_attribute ("name");
    const xmlpp::Attribute *variable_attribute = nodeElement->get_attribute ("variable");
    const xmlpp::Attribute *operation_attribute = nodeElement->get_attribute ("operation");
    const xmlpp::Attribute *variable2_attribute = nodeElement->get_attribute ("variable2");

    if (name_attribute)
    {
      cout << "Attribute name = " << name_attribute->get_value () << endl;
    }

    if (variable_attribute)
    {
      cout << "Attribute variable = " << variable_attribute->get_value () << endl;
    }

    // ignore currently, and use "equals" as default...
    if (operation_attribute)
    {
      cout << "Attribute operation = " << operation_attribute->get_value () << endl;
    }

    if (variable2_attribute)
    {
      cout << "Attribute operation = " << variable2_attribute->get_value () << endl;
    }

    GlobalVariables &global = GlobalVariables::instance ();
    AbstractVariable *av = global.getVariable (variable2_attribute->get_value ());
    assert (av);
    
    Condition *cond = new Condition ();
    cond->addComparison (variable_attribute->get_value (), av);

    // temporary save condition pointer for later reference in transition table
    mConditionNameMapper[name_attribute->get_value ()] = cond;
  }
}

void XMLLoader::parseActionsNode (const xmlpp::Node * node)
{
  const xmlpp::ContentNode * nodeContent = dynamic_cast < const xmlpp::ContentNode * >(node);
  const xmlpp::TextNode * nodeText = dynamic_cast < const xmlpp::TextNode * >(node);
  const xmlpp::CommentNode * nodeComment = dynamic_cast < const xmlpp::CommentNode * >(node);

  if (nodeText && nodeText->is_white_space ())	//Let's ignore the indenting
    return;

  Glib::ustring nodename = node->get_name ();

  if (!nodeText && !nodeComment && !nodename.empty ())	//Let's not say "name: text".
  {
    if (nodename == "actions")
    {
      //Recurse through child nodes:
      xmlpp::Node::NodeList list = node->get_children ();
      for (xmlpp::Node::NodeList::iterator iter = list.begin ();
           iter != list.end (); ++iter)
      {
        parseActionNode (*iter);
      }
    }
  }
}

void XMLLoader::parseActionNode (const xmlpp::Node * node)
{
  const xmlpp::TextNode * nodeText = dynamic_cast < const xmlpp::TextNode * >(node);
  const xmlpp::Element * nodeElement = dynamic_cast < const xmlpp::Element * >(node);
  Action *action = NULL;
  
  if (nodeText && nodeText->is_white_space ())	//Let's ignore the indenting
    return;

  Glib::ustring nodename = node->get_name ();

  if (nodename == "action")
  {
    cout << "Node = " << node->get_name () << endl;

    const xmlpp::Attribute *name_attribute = nodeElement->get_attribute ("name");
    const xmlpp::Attribute *type_attribute = nodeElement->get_attribute ("type");
    const xmlpp::Attribute *event_attribute = nodeElement->get_attribute ("event");
    const xmlpp::Attribute *variable_attribute = nodeElement->get_attribute ("variable");
    const xmlpp::Attribute *copy_attribute = nodeElement->get_attribute ("copy");;
    
    if (name_attribute)
    {
      cout << "Attribute name = " << name_attribute->get_value () << endl;
    }

    if (event_attribute)
    {
      cout << "Attribute event = " << event_attribute->get_value () << endl;
    }
    if (type_attribute->get_value () == "FireEventAction")
    {
      // TODO: check if event is available/useful
      action = new FireEventAction (event_attribute->get_value ());
      mActionNameMapper[name_attribute->get_value ()] = action;
    }
    else if (type_attribute->get_value () == "ChangeVariableAction")
    {
      GlobalVariables &global = GlobalVariables::instance ();
      AbstractVariable *av = global.getVariable (copy_attribute->get_value ());
      assert (av);
      
      action = new ChangeVariableAction (variable_attribute->get_value (), av);
      mActionNameMapper[name_attribute->get_value ()] = action;
    }
    else
    {
      cerr << "unknow action" << endl;
      assert (false);
    }

    assert (action);
    addAction (action);
  }
}

void XMLLoader::parseStatesNode (const xmlpp::Node * node)
{
  const xmlpp::ContentNode * nodeContent = dynamic_cast < const xmlpp::ContentNode * >(node);
  const xmlpp::TextNode * nodeText = dynamic_cast < const xmlpp::TextNode * >(node);
  const xmlpp::CommentNode * nodeComment = dynamic_cast < const xmlpp::CommentNode * >(node);

  if (nodeText && nodeText->is_white_space ())	//Let's ignore the indenting
    return;

  Glib::ustring nodename = node->get_name ();

  if (!nodeText && !nodeComment && !nodename.empty ())	//Let's not say "name: text".
  {
    if (nodename == "states")
    {
      // Recurse through child nodes (create Index)
      unsigned int i = 0;
      xmlpp::Node::NodeList list = node->get_children ();
      for (xmlpp::Node::NodeList::iterator iter = list.begin ();
           iter != list.end ();
           ++iter)
      {
        parseStateNodeIndex (*iter, i);
      }

      // Recurse through child nodes
      list = node->get_children ();
      for (xmlpp::Node::NodeList::iterator iter = list.begin ();
           iter != list.end ();
           ++iter)
      {
        parseStateNode (*iter);
      }
    }
  }
}

void XMLLoader::parseStateNodeIndex (const xmlpp::Node * node, unsigned int &i)
{
  const xmlpp::TextNode * nodeText = dynamic_cast < const xmlpp::TextNode * >(node);
  const xmlpp::Element * nodeElement = dynamic_cast < const xmlpp::Element * >(node);
  
  if (nodeText && nodeText->is_white_space ())	//Let's ignore the indenting
    return;

  Glib::ustring nodename = node->get_name ();

  if (nodename == "state")
  {
    cout << "Node (Index) = " << node->get_name () << endl;

    const xmlpp::Attribute *name_attribute = nodeElement->get_attribute ("name");
    
    if (name_attribute)
    {
      const Glib::ustring &name = name_attribute->get_value ();
      cout << "Attribute name (Index) = " << name << endl;
      ++i; // modifies also outside of function!
      mStateNameMapper[name] = i;
    }      
  }
}

void XMLLoader::parseStateNode (const xmlpp::Node * node)
{
  const xmlpp::TextNode * nodeText = dynamic_cast < const xmlpp::TextNode * >(node);
  const xmlpp::Element * nodeElement = dynamic_cast < const xmlpp::Element * >(node);
  
  if (nodeText && nodeText->is_white_space ())// Let's ignore the indenting
    return;

  Glib::ustring nodename = node->get_name ();

  if (nodename == "state")
  {
    cout << "Node = " << node->get_name () << endl;

    const xmlpp::Attribute *name_attribute = nodeElement->get_attribute ("name");
    const xmlpp::Attribute *type_attribute = nodeElement->get_attribute ("type");
    const xmlpp::Attribute *parent_attribute = nodeElement->get_attribute ("parent");
    const xmlpp::Attribute *view_attribute = nodeElement->get_attribute ("view");

    State *state = NULL;
    CompoundState *parentState = NULL;
    int parentNum = 0;

    if (name_attribute)
    {
      cout << "Attribute name = " << name_attribute->get_value () << endl;
    }
    else
    {
      // throw exception
    }
    
    if (parent_attribute)
    {
      const Glib::ustring &parent = parent_attribute->get_value ();
      cout << "Attribute parent = " << parent << endl;
      // TODO: better use find() to detect if not found in map
      parentNum = mStateNameMapper[parent];

      // TODO: think about detecting root if no parent is defined...
      if (parentNum != 0) // negative detection of root compound
      {
        // TODO: better use find() to detect if not found in map
        parentState = static_cast <CompoundState*> (mStateList[parentNum-1]);
        assert (parentState);
      }
    }
    else
    {
      // throw Exception
    }

    // check type and throw exception    
    if (view_attribute)
    {
      cout << "Attribute view = " << view_attribute->get_value () << endl;
    }

    if (type_attribute)
    {
      const Glib::ustring &type = type_attribute->get_value ();

      cout << "Attribute type = " << type << endl;

      if (type == "CompoundState")
      {
        if (parentNum == 0) // detection of root compound
        {
          state = new CompoundState ();
        }
        else
        {
          state = new CompoundState (parentState);
        }
      }
      else if (type == "SimpleState")
      {
        state = new SimpleState (parentState);
      }
      else if (type == "HistoryState")
      {
        HistoryState *historyState = new HistoryState (parentState);
        
        parentState->setHistory (historyState);
        state = historyState;
      }
      else if (type == "DecisionState")
      {
        state = new DecisionState (parentState);
      }
      else if (type == "ViewState")
      {
        // TODO: better use find() to detect if not found in map
        int viewNum = mViewNameMapper[view_attribute->get_value ()];
        View *view = mViewList[viewNum];
        state = new ViewState (parentState, *view);
      }
      else
      {
        // throw exception
      }
      
      assert (state);
      
      state->setID (mStateNameMapper[name_attribute->get_value ()]);
      state->setName (name_attribute->get_value ());

      // Recurse through child nodes
      xmlpp::Node::NodeList list = node->get_children ();
      for (xmlpp::Node::NodeList::iterator iter = list.begin ();
           iter != list.end (); ++iter)
      {
        parseStateActionsNode (*iter, state);
      }
      
      addState (state);
    }
    else
    {
      // throw Exception
    }
  }
}

void XMLLoader::parseStateActionsNode (const xmlpp::Node * node, State *state)
{
  const xmlpp::ContentNode * nodeContent = dynamic_cast < const xmlpp::ContentNode * >(node);
  const xmlpp::TextNode * nodeText = dynamic_cast < const xmlpp::TextNode * >(node);
  const xmlpp::CommentNode * nodeComment = dynamic_cast < const xmlpp::CommentNode * >(node);

  if (nodeText && nodeText->is_white_space ())	//Let's ignore the indenting
    return;

  Glib::ustring nodename = node->get_name ();

  if (!nodeText && !nodeComment && !nodename.empty ())	//Let's not say "name: text".
  {
    if (nodename == "actions")
    {
      //Recurse through child nodes:
      xmlpp::Node::NodeList list = node->get_children ();
      for (xmlpp::Node::NodeList::iterator iter = list.begin ();
           iter != list.end (); ++iter)
      {
        parseStateActionNode (*iter, state);
      }
    }
  }
}

void XMLLoader::parseStateActionNode (const xmlpp::Node * node, State *state)
{
  const xmlpp::TextNode * nodeText = dynamic_cast < const xmlpp::TextNode * >(node);
  const xmlpp::Element * nodeElement = dynamic_cast < const xmlpp::Element * >(node);
  
  if (nodeText && nodeText->is_white_space ())	//Let's ignore the indenting
    return;

  Glib::ustring nodename = node->get_name ();

  if (nodename == "action")
  {
    cout << "Node = " << node->get_name () << endl;

    const xmlpp::Attribute *ref_attribute = nodeElement->get_attribute ("ref");
    const xmlpp::Attribute *when_attribute = nodeElement->get_attribute ("when");

    if (ref_attribute)
    {
      cout << "Attribute ref = " << ref_attribute->get_value () << endl;
    }

    if (when_attribute)
    {
      cout << "Attribute when = " << when_attribute->get_value () << endl;
    }

    // TODO: use find...
    Action *action = mActionNameMapper[ref_attribute->get_value ()];
    assert (action);
    
    if (when_attribute->get_value () == "enter")
    {
      state->addEntryAction (action);
    }
    else if (when_attribute->get_value () == "leave")
    {
      state->addExitAction (action);
    }
    else
    {
      assert (false);
    }
  }
}

void XMLLoader::parseTransitionsNode (const xmlpp::Node * node)
{
  const xmlpp::ContentNode * nodeContent = dynamic_cast < const xmlpp::ContentNode * >(node);
  const xmlpp::TextNode * nodeText = dynamic_cast < const xmlpp::TextNode * >(node);
  const xmlpp::CommentNode * nodeComment = dynamic_cast < const xmlpp::CommentNode * >(node);

  if (nodeText && nodeText->is_white_space ())	//Let's ignore the indenting
    return;

  Glib::ustring nodename = node->get_name ();

  if (!nodeText && !nodeComment && !nodename.empty ())	//Let's not say "name: text".
  {
    if (nodename == "transitions")
    {
      //Recurse through child nodes:
      xmlpp::Node::NodeList list = node->get_children ();
      for (xmlpp::Node::NodeList::iterator iter = list.begin ();
           iter != list.end (); ++iter)
      {
        parseTransitionNode (*iter);
      }
    }
  }
}

void XMLLoader::parseTransitionNode (const xmlpp::Node * node)
{
  const xmlpp::TextNode * nodeText = dynamic_cast < const xmlpp::TextNode * >(node);
  const xmlpp::Element * nodeElement = dynamic_cast < const xmlpp::Element * >(node);
  
  if (nodeText && nodeText->is_white_space ())	//Let's ignore the indenting
    return;

  Glib::ustring nodename = node->get_name ();

  if (!nodename.empty ())
  {
    cout << "Node = " << node->get_name () << endl;

    const xmlpp::Attribute *from_attribute = nodeElement->get_attribute ("from");
    const xmlpp::Attribute *to_attribute = nodeElement->get_attribute ("to");
    const xmlpp::Attribute *event_attribute = nodeElement->get_attribute ("event");
    const xmlpp::Attribute *decision_attribute = nodeElement->get_attribute ("decision");

    int fromStateNum = 0;
    int toStateNum = 0;
    State *fromState = NULL;
    State *toState = NULL;
    Transition *trans = NULL;
    
    if (from_attribute)
    {
      cout << "Attribute from = " << from_attribute->get_value () << endl;

      // TODO: better use find() to detect if not found in map
      fromStateNum = mStateNameMapper[from_attribute->get_value ()];
      fromState = mStateList[fromStateNum-1];
    }
    else
    {
      // throw exception
    }
    
    if (to_attribute)
    {
      cout << "Attribute to = " << to_attribute->get_value () << endl;

      // TODO: better use find() to detect if not found in map
      toStateNum = mStateNameMapper[to_attribute->get_value ()];
      toState = mStateList[toStateNum-1];
    }
    else
    {
      // throw exception
    }
    
    if (event_attribute)
    {
      cout << "Attribute event = " << event_attribute->get_value () << endl;
      trans = new Transition (toState, findMapingEvent (event_attribute->get_value ()));
    }
    else 
    {
      trans = new Transition (toState);
    }
    
    if (decision_attribute)
    {
      // TODO: maybe there's a better idea to solve this with correct polymorphy design...
      DecisionState* decisionState = static_cast <DecisionState*> (fromState);
      
      cout << "Attribute decision = " << decision_attribute->get_value () << endl;
      Condition *cond = mConditionNameMapper[decision_attribute->get_value ()];
      std::pair <Condition*, Transition*> conTrans (cond, trans);
      decisionState->addConditionTransition (conTrans);
    }
    else
    {
      fromState->addLeaveTransition (*trans);
    }
  }
}

void XMLLoader::parseViewsNode (const xmlpp::Node * node)
{
  const xmlpp::ContentNode * nodeContent = dynamic_cast < const xmlpp::ContentNode * >(node);
  const xmlpp::TextNode * nodeText = dynamic_cast < const xmlpp::TextNode * >(node);
  const xmlpp::CommentNode * nodeComment = dynamic_cast < const xmlpp::CommentNode * >(node);
  const xmlpp::Element * nodeElement = dynamic_cast < const xmlpp::Element * >(node);

  if (nodeText && nodeText->is_white_space ())	//Let's ignore the indenting
    return;

  Glib::ustring nodename = node->get_name ();

  if (!nodeText && !nodeComment && !nodename.empty ())	//Let's not say "name: text".
  {
    if (nodename == "views")
    {
      const xmlpp::Attribute *plugin_attribute = nodeElement->get_attribute ("plugin");

      if (plugin_attribute)
      {
        cout << "Attribute plugin = " << plugin_attribute->get_value () << endl;
      }
      else
      {
        // throw exception
      }
      
      // Recurse through child nodes
      unsigned int i = 0;
      xmlpp::Node::NodeList list = node->get_children ();
      for (xmlpp::Node::NodeList::iterator iter = list.begin ();
           iter != list.end (); ++iter)
      {
        parseViewNode (*iter, plugin_attribute->get_value (), i);
      }
    }
  }
}

void XMLLoader::parseViewNode (const xmlpp::Node * node, const Glib::ustring &plugin, unsigned int &i)
{
  const xmlpp::ContentNode * nodeContent = dynamic_cast < const xmlpp::ContentNode * >(node);
  const xmlpp::TextNode * nodeText = dynamic_cast < const xmlpp::TextNode * >(node);
  const xmlpp::CommentNode * nodeComment = dynamic_cast < const xmlpp::CommentNode * >(node);
  const xmlpp::Element * nodeElement = dynamic_cast < const xmlpp::Element * >(node);
  View *view = NULL;

  if (nodeText && nodeText->is_white_space ())	// Let's ignore the indenting
    return;

  Glib::ustring nodename = node->get_name ();

  if (!nodeText && !nodeComment && !nodename.empty ())	// Let's not say "name: text".
  {
    if (nodename == "view")
    {
      const xmlpp::Attribute *name_attribute = nodeElement->get_attribute ("name");

      if (name_attribute)
      {
        cout << "Attribute name = " << name_attribute->get_value () << endl;
        mViewNameMapper[name_attribute->get_value ()] = i;
        ++i; // modifies also outside of function!
      }
      else
      {
        // throw exception
      }

      list <std::string> params;
      
      // Recurse through child nodes
      xmlpp::Node::NodeList list = node->get_children ();
      for (xmlpp::Node::NodeList::iterator iter = list.begin ();
           iter != list.end (); ++iter)
      {
        parseViewParamsNode (*iter, params);
      }

      string pluginFile (searchPluginFile ("views", plugin));  
      view = loadView (pluginFile, mContext, params);
      mViewList.push_back (view);
      
      // Recurse through child nodes
      list = node->get_children ();
      for (xmlpp::Node::NodeList::iterator iter = list.begin ();
           iter != list.end (); ++iter)
      {
        parseViewMappingsNode (*iter, view);
      }

      // Recurse through child nodes
      list = node->get_children ();
      for (xmlpp::Node::NodeList::iterator iter = list.begin ();
           iter != list.end (); ++iter)
      {
        parseViewWidgetsNode (*iter, view);
      }
    }
  }
}

void XMLLoader::parseTypesNode (const xmlpp::Node * node)
{
  const xmlpp::ContentNode * nodeContent = dynamic_cast < const xmlpp::ContentNode * >(node);
  const xmlpp::TextNode * nodeText = dynamic_cast < const xmlpp::TextNode * >(node);
  const xmlpp::CommentNode * nodeComment = dynamic_cast < const xmlpp::CommentNode * >(node);
  const xmlpp::Element * nodeElement = dynamic_cast < const xmlpp::Element * >(node);

  if (nodeText && nodeText->is_white_space ())	//Let's ignore the indenting
    return;

  Glib::ustring nodename = node->get_name ();

  if (!nodeText && !nodeComment && !nodename.empty ())	//Let's not say "name: text".
  {
    if (nodename == "types")
    {      
      // Recurse through child nodes
      //unsigned int i = 0;
      xmlpp::Node::NodeList list = node->get_children ();
      for (xmlpp::Node::NodeList::iterator iter = list.begin ();
           iter != list.end (); ++iter)
      {
        parseTypeNode (*iter/*, plugin_attribute->get_value (), i*/);
      }
    }
  }
}

void XMLLoader::parseTypeNode (const xmlpp::Node * node/*, const Glib::ustring &plugin, unsigned int &i*/)
{
  const xmlpp::ContentNode * nodeContent = dynamic_cast < const xmlpp::ContentNode * >(node);
  const xmlpp::TextNode * nodeText = dynamic_cast < const xmlpp::TextNode * >(node);
  const xmlpp::CommentNode * nodeComment = dynamic_cast < const xmlpp::CommentNode * >(node);
  const xmlpp::Element * nodeElement = dynamic_cast < const xmlpp::Element * >(node);
  View *view = NULL;

  if (nodeText && nodeText->is_white_space ())	// Let's ignore the indenting
    return;

  Glib::ustring nodename = node->get_name ();

  if (!nodeText && !nodeComment && !nodename.empty ())	// Let's not say "name: text".
  {
    if (nodename == "type")
    {
      const xmlpp::Attribute *name_attribute = nodeElement->get_attribute ("name");
      const xmlpp::Attribute *plugin_attribute = nodeElement->get_attribute ("plugin");

      if (name_attribute)
      {
        cout << "Attribute name = " << name_attribute->get_value () << endl;

      }
      else
      {
        // throw exception
      }

      if (plugin_attribute)
      {
        cout << "Attribute plugin = " << plugin_attribute->get_value () << endl;
      }
      else
      {
        // Recurse through child nodes
        xmlpp::Node::NodeList list = node->get_children ();
        for (xmlpp::Node::NodeList::iterator iter = list.begin ();
             iter != list.end (); ++iter)
        {
          parseTypeElementNode (*iter);
        }
      }
    }
  }
}

void XMLLoader::parseTypeElementNode (const xmlpp::Node * node)
{
  const xmlpp::TextNode * nodeText = dynamic_cast < const xmlpp::TextNode * >(node);
  const xmlpp::Element * nodeElement = dynamic_cast < const xmlpp::Element * >(node);
  
  if (nodeText && nodeText->is_white_space ())	//Let's ignore the indenting
    return;

  Glib::ustring nodename = node->get_name ();

  if (nodename == "element")
  {
    cout << "Node = " << nodename << endl;

    const xmlpp::Attribute *name_attribute = nodeElement->get_attribute ("name");
    const xmlpp::Attribute *type_attribute = nodeElement->get_attribute ("type");

    if (type_attribute)
    {
      cout << "Attribute type = " << type_attribute->get_value () << endl;
    }
  }
}


void XMLLoader::parseViewParamsNode (const xmlpp::Node * node, std::list <std::string> &params)
{
  const xmlpp::ContentNode * nodeContent = dynamic_cast < const xmlpp::ContentNode * >(node);
  const xmlpp::TextNode * nodeText = dynamic_cast < const xmlpp::TextNode * >(node);
  const xmlpp::CommentNode * nodeComment = dynamic_cast < const xmlpp::CommentNode * >(node);

  if (nodeText && nodeText->is_white_space ())	//Let's ignore the indenting
    return;

  Glib::ustring nodename = node->get_name ();

  if (!nodeText && !nodeComment && !nodename.empty ())	//Let's not say "name: text".
  {
    if (nodename == "params")
    {
      // Recurse through child nodes
      xmlpp::Node::NodeList list = node->get_children ();
      for (xmlpp::Node::NodeList::iterator iter = list.begin ();
           iter != list.end (); ++iter)
      {
        parseViewParamNode (*iter, params);
      }
    }
  }
}

void XMLLoader::parseViewParamNode (const xmlpp::Node * node, std::list <std::string> &params)
{
  const xmlpp::TextNode * nodeText = dynamic_cast < const xmlpp::TextNode * >(node);
  const xmlpp::Element * nodeElement = dynamic_cast < const xmlpp::Element * >(node);
  
  if (nodeText && nodeText->is_white_space ())	//Let's ignore the indenting
    return;

  Glib::ustring nodename = node->get_name ();

  if (!nodename.empty ())
  {
    cout << "Node = " << node->get_name () << endl;

    const xmlpp::Attribute *value_attribute = nodeElement->get_attribute ("value");

    if (value_attribute)
    {
      cout << "Attribute value = " << value_attribute->get_value () << endl;

      params.push_back (value_attribute->get_value ());
    }
  }
}

void XMLLoader::parseViewMappingsNode (const xmlpp::Node * node, View *view)
{
  const xmlpp::ContentNode * nodeContent = dynamic_cast < const xmlpp::ContentNode * >(node);
  const xmlpp::TextNode * nodeText = dynamic_cast < const xmlpp::TextNode * >(node);
  const xmlpp::CommentNode * nodeComment = dynamic_cast < const xmlpp::CommentNode * >(node);

  if (nodeText && nodeText->is_white_space ())	//Let's ignore the indenting
    return;

  Glib::ustring nodename = node->get_name ();

  if (!nodeText && !nodeComment && !nodename.empty ())	//Let's not say "name: text".
  {
    if (nodename == "mappings")
    {
      // Recurse through child nodes
      xmlpp::Node::NodeList list = node->get_children ();
      for (xmlpp::Node::NodeList::iterator iter = list.begin ();
           iter != list.end (); ++iter)
      {
        parseViewMapNode (*iter, view);
      }
    }
  }
}

void XMLLoader::parseViewMapNode (const xmlpp::Node * node, View *view)
{
  const xmlpp::TextNode * nodeText = dynamic_cast < const xmlpp::TextNode * >(node);
  const xmlpp::Element * nodeElement = dynamic_cast < const xmlpp::Element * >(node);
  
  if (nodeText && nodeText->is_white_space ())	//Let's ignore the indenting
    return;

  Glib::ustring nodename = node->get_name ();

  if (nodename == "map")
  {
    cout << "Node = " << node->get_name () << endl;

    const xmlpp::Attribute *from_attribute = nodeElement->get_attribute ("from");
    const xmlpp::Attribute *to_attribute = nodeElement->get_attribute ("to");

    if (from_attribute)
    {
      cout << "Attribute from = " << from_attribute->get_value () << endl;
    }
    else
    {
      // throw exception
    }  
    
    if (to_attribute)
    {
      cout << "Attribute to = " << to_attribute->get_value () << endl;
    }
    else
    {
      // throw exception
    }

    view->addEventMapping (findMapingEvent (from_attribute->get_value ()), 
                           findMapingEvent (to_attribute->get_value ()));
  }
}

void XMLLoader::parseViewWidgetsNode (const xmlpp::Node * node, View *view)
{
  const xmlpp::ContentNode * nodeContent = dynamic_cast < const xmlpp::ContentNode * >(node);
  const xmlpp::TextNode * nodeText = dynamic_cast < const xmlpp::TextNode * >(node);
  const xmlpp::CommentNode * nodeComment = dynamic_cast < const xmlpp::CommentNode * >(node);

  if (nodeText && nodeText->is_white_space ())	//Let's ignore the indenting
    return;

  Glib::ustring nodename = node->get_name ();

  if (!nodeText && !nodeComment && !nodename.empty ())	//Let's not say "name: text".
  {
    if (nodename == "widgets")
    {
      // Recurse through child nodes
      xmlpp::Node::NodeList list = node->get_children ();
      for (xmlpp::Node::NodeList::iterator iter = list.begin ();
           iter != list.end (); ++iter)
      {
        parseViewWidgetNode (*iter, view);
      }
    }
  }
}

void XMLLoader::parseViewWidgetNode (const xmlpp::Node * node, View *view)
{
  const xmlpp::TextNode * nodeText = dynamic_cast < const xmlpp::TextNode * >(node);
  const xmlpp::Element * nodeElement = dynamic_cast < const xmlpp::Element * >(node);
  
  if (nodeText && nodeText->is_white_space ())	//Let's ignore the indenting
    return;

  Glib::ustring nodename = node->get_name ();

  if (nodename == "widget")
  {
    cout << "Node = " << node->get_name () << endl;

    const xmlpp::Attribute *name_attribute = nodeElement->get_attribute ("name");
    const xmlpp::Attribute *variable_attribute = nodeElement->get_attribute ("variable");

    if (name_attribute)
    {
      cout << "Attribute name = " << name_attribute->get_value () << endl;
    }
    else
    {
      // throw exception
    }  
    
    if (variable_attribute)
    {
      cout << "Attribute variable = " << variable_attribute->get_value () << endl;
    }
    else
    {
      // throw exception
    }

    view->addWidget (Widget (name_attribute->get_value (), variable_attribute->get_value ()));
  }
}

/*****************************/
/* Plugin needed C functions */
/*****************************/

PLUGIN_EXPORT XMLLoader *plugin_create ()
{
  return new XMLLoader ();
}

PLUGIN_EXPORT void plugin_destroy (Loader *plugin)
{
  delete plugin;
}

PLUGIN_EXPORT const char *get_plugin_type ()
{
  return type;
}

PLUGIN_EXPORT unsigned int get_plugin_major_version ()
{
  return major_version;
}
