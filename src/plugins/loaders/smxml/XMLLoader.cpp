#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include "XMLLoader.h"

using namespace std;

static const char* type = "Loader";
static const unsigned int major_version = 1;
static const unsigned int minor_version = 1;

XMLLoader::XMLLoader ()
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

bool XMLLoader::load (Context *context, const std::string &smDir)
{
  loadSiteFile (smDir);
}

void XMLLoader::unload ()
{

}

void XMLLoader::parseRootNode (const xmlpp::Node * node)
{
  const xmlpp::ContentNode * nodeContent = dynamic_cast < const xmlpp::ContentNode * >(node);
  const xmlpp::TextNode * nodeText = dynamic_cast < const xmlpp::TextNode * >(node);
  const xmlpp::CommentNode * nodeComment = dynamic_cast < const xmlpp::CommentNode * >(node);
  bool ret = false;

  if (nodeText && nodeText->is_white_space ())	//Let's ignore the indenting
    return;

  Glib::ustring nodename = node->get_name ();

  if (!nodeText && !nodeComment && !nodename.empty ())	//Let's not say "name: text".
  {
    if (nodename == "stateval")
    {
      //Recurse through child nodes:
      xmlpp::Node::NodeList list = node->get_children ();
      for (xmlpp::Node::NodeList::iterator iter = list.begin ();
           iter != list.end (); ++iter)
      {
        parseBlockNode (*iter);
      }
    }
  }
}

void XMLLoader::parseBlockNode (const xmlpp::Node * node)
{
  const xmlpp::ContentNode * nodeContent = dynamic_cast <const xmlpp::ContentNode *> (node);
  const xmlpp::TextNode * nodeText = dynamic_cast <const xmlpp::TextNode * > (node);
  const xmlpp::CommentNode * nodeComment = dynamic_cast <const xmlpp::CommentNode *> (node);
  bool ret = false;

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
  bool ret = false;

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
  bool ret = false;

  if (nodeText && nodeText->is_white_space ())	//Let's ignore the indenting
    return;

  Glib::ustring nodename = node->get_name ();

  if (!nodeText && !nodeComment && !nodename.empty ())	//Let's not say "name: text".
  {
    if (nodename == "states")
    {
      //Recurse through child nodes:
      xmlpp::Node::NodeList list = node->get_children ();
      for (xmlpp::Node::NodeList::iterator iter = list.begin ();
           iter != list.end (); ++iter)
      {
        parseStateNode (*iter);
      }
    }
  }
}

void XMLLoader::parseStateNode (const xmlpp::Node * node)
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
    const xmlpp::Attribute *type_attribute = nodeElement->get_attribute ("type");
    const xmlpp::Attribute *parent_attribute = nodeElement->get_attribute ("parent");
    const xmlpp::Attribute *history_attribute = nodeElement->get_attribute ("history");
    const xmlpp::Attribute *view_attribute = nodeElement->get_attribute ("view");

    if (name_attribute)
    {
      cout << "Attribute name = " << name_attribute->get_value () << endl;
    }
    if (type_attribute)
    {
      cout << "Attribute type = " << type_attribute->get_value () << endl;
    }
    if (parent_attribute)
    {
      cout << "Attribute parent = " << parent_attribute->get_value () << endl;
    }
    if (history_attribute)
    {
      cout << "Attribute history = " << history_attribute->get_value () << endl;
    }
    if (view_attribute)
    {
      cout << "Attribute view = " << view_attribute->get_value () << endl;
    }
      
  }
}

void XMLLoader::parseTransitionsNode (const xmlpp::Node * node)
{
  const xmlpp::ContentNode * nodeContent = dynamic_cast < const xmlpp::ContentNode * >(node);
  const xmlpp::TextNode * nodeText = dynamic_cast < const xmlpp::TextNode * >(node);
  const xmlpp::CommentNode * nodeComment = dynamic_cast < const xmlpp::CommentNode * >(node);
  bool ret = false;

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

    if (from_attribute)
    {
      cout << "Attribute from = " << from_attribute->get_value () << endl;
    }
    if (to_attribute)
    {
      cout << "Attribute to = " << to_attribute->get_value () << endl;
    }
    if (event_attribute)
    {
      cout << "Attribute event = " << event_attribute->get_value () << endl;
    }
      
  }
}

void XMLLoader::parseViewsNode (const xmlpp::Node * node)
{
  const xmlpp::ContentNode * nodeContent = dynamic_cast < const xmlpp::ContentNode * >(node);
  const xmlpp::TextNode * nodeText = dynamic_cast < const xmlpp::TextNode * >(node);
  const xmlpp::CommentNode * nodeComment = dynamic_cast < const xmlpp::CommentNode * >(node);
  const xmlpp::Element * nodeElement = dynamic_cast < const xmlpp::Element * >(node);
  bool ret = false;

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
      
      //Recurse through child nodes:
      xmlpp::Node::NodeList list = node->get_children ();
      for (xmlpp::Node::NodeList::iterator iter = list.begin ();
           iter != list.end (); ++iter)
      {
        parseViewNode (*iter);
      }
    }
  }
}

void XMLLoader::parseViewNode (const xmlpp::Node * node)
{
  const xmlpp::ContentNode * nodeContent = dynamic_cast < const xmlpp::ContentNode * >(node);
  const xmlpp::TextNode * nodeText = dynamic_cast < const xmlpp::TextNode * >(node);
  const xmlpp::CommentNode * nodeComment = dynamic_cast < const xmlpp::CommentNode * >(node);
  const xmlpp::Element * nodeElement = dynamic_cast < const xmlpp::Element * >(node);
  bool ret = false;

  if (nodeText && nodeText->is_white_space ())	//Let's ignore the indenting
    return;

  Glib::ustring nodename = node->get_name ();

  if (!nodeText && !nodeComment && !nodename.empty ())	//Let's not say "name: text".
  {
    if (nodename == "view")
    {
      const xmlpp::Attribute *name_attribute = nodeElement->get_attribute ("name");
      const xmlpp::Attribute *params_attribute = nodeElement->get_attribute ("params");

      if (name_attribute)
      {
        cout << "Attribute name = " << name_attribute->get_value () << endl;
      }
      if (params_attribute)
      {
        cout << "Attribute params = " << params_attribute->get_value () << endl;
      }
      
      //Recurse through child nodes:
      xmlpp::Node::NodeList list = node->get_children ();
      for (xmlpp::Node::NodeList::iterator iter = list.begin ();
           iter != list.end (); ++iter)
      {
        parseViewMapNode (*iter);
      }
    }
  }
}

void XMLLoader::parseViewMapNode (const xmlpp::Node * node)
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

    if (from_attribute)
    {
      cout << "Attribute from = " << from_attribute->get_value () << endl;
    }
    if (to_attribute)
    {
      cout << "Attribute to = " << to_attribute->get_value () << endl;
    }

  }
}

void XMLLoader::loadSiteFile (const Glib::ustring & filepath)
{  
  try
  {
    xmlpp::DomParser parser;
    parser.set_validate (false);	// TODO: activate DTD later...
    parser.set_substitute_entities ();	//We just want the text to be resolved/unescaped automatically.
    parser.parse_file (filepath);
    if (parser)
    {
      //Walk the tree:
      const xmlpp::Node * pNode = parser.get_document ()->get_root_node ();	//deleted by DomParser.
      parseRootNode (pNode);
    }
  }
  catch (const exception &ex)
  {
    std::cout << "Exception caught: " << ex.what () << std::endl;
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
