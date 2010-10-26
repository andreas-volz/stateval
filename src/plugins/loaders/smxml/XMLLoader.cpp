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
  delete_stl_container <std::vector <View*>, View*> (mViewList);

  // free mStateList
  delete_stl_container <std::vector <State*>, State*> (mStateList);
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

void XMLLoader::parseBlockNode (const xmlpp::Node * node)
{
  const xmlpp::ContentNode * nodeContent = dynamic_cast <const xmlpp::ContentNode *> (node);
  const xmlpp::TextNode * nodeText = dynamic_cast <const xmlpp::TextNode * > (node);
  const xmlpp::CommentNode * nodeComment = dynamic_cast <const xmlpp::CommentNode *> (node);

  if (nodeText && nodeText->is_white_space ())	//Let's ignore the indenting
    return;

  Glib::ustring nodename = node->get_name ();
  
  cout << "Parse Node: " << nodename << endl;

  if (!nodeText && !nodeComment && !nodename.empty ())	//Let's not say "name: text".
  {
    if (nodename == "events")
    {
      parseEventsNode (node);
    }
    else if (nodename == "states")
    {
      parseStatesNode (node);
    }
    else if (nodename == "transitions")
    {
      parseTransitionsNode (node);
    }
    else if (nodename == "views")
    {
      parseViewsNode (node);
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

  if (!nodename.empty ())
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

  if (!nodename.empty ())
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

  if (!nodename.empty ())
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
        // TODO: better use find() to detect if not found in map
        //int historyNum = mStateNameMapper [history_attribute->get_value ()];
        //State *stateTrans = mStateList[historyNum-1];
        HistoryState *historyState = new HistoryState (parentState);
        
        parentState->setHistory (historyState);
        //historyState->changeTransition (stateTrans);
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
      
      addState (state);
    }
    else
    {
      // throw Exception
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

    fromState->addLeaveTransition (*trans);      
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
        parseViewMapNode (*iter, view);
      }
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
        parseParamNode (*iter, params);
      }
    }
  }
}

void XMLLoader::parseParamNode (const xmlpp::Node * node, std::list <std::string> &params)
{
  const xmlpp::TextNode * nodeText = dynamic_cast < const xmlpp::TextNode * >(node);
  const xmlpp::Element * nodeElement = dynamic_cast < const xmlpp::Element * >(node);
  
  if (nodeText && nodeText->is_white_space ())	//Let's ignore the indenting
    return;

  Glib::ustring nodename = node->get_name ();

  if (!nodename.empty ())
  {
    cout << "Node = " << node->get_name () << endl;

    const xmlpp::Attribute *name_attribute = nodeElement->get_attribute ("value");

    if (name_attribute)
    {
      cout << "Attribute value = " << name_attribute->get_value () << endl;

      params.push_back (name_attribute->get_value ());
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
