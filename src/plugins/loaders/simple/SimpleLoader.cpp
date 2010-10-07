#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

/* STD */
#include <iostream>
#include <cassert>

/* stateval */
#include "stateval/CompoundState.h"
#include "stateval/HistoryState.h"
#include "stateval/DecisionState.h"
#include "stateval/ViewState.h"

/* Project */
#include "SimpleLoader.h"
#include "FileReader.h"
#include "localUtil.h"
#include "stringUtil.h"

using namespace std;

static const char* type = "Loader";
static const unsigned int major_version = 1;
static const unsigned int minor_version = 1;

SimpleLoader::SimpleLoader ()
{
}

SimpleLoader::~SimpleLoader ()
{
  unload ();
}

const std::string SimpleLoader::getType ()
{
  return type;
}

const unsigned int SimpleLoader::getMajorVersion ()
{
  return major_version;
}

const unsigned int SimpleLoader::getMinorVersion ()
{
  return minor_version;
}

bool SimpleLoader::load (const std::string &smDir)
{
  FileReader fileReader;
  
  unload ();
  
  // load events
  fileReader.open (smDir + "/" + "events");

  if (!fileReader)
    return false;

  fromEventsStream (fileReader);
  fileReader.close ();

  // load views
  fileReader.open (smDir + "/" + "views");

  if (!fileReader)
    return false;

  fromViewsStream (fileReader, smDir);
  fileReader.close ();
  
  // load states
  fileReader.open (smDir + "/" + "states");

  if (!fileReader)
    return false;

  fromStatesStream (fileReader);
  fileReader.close ();
  
  // load transitions
  fileReader.open (smDir + "/" + "transitions");

  if (!fileReader)
    return false;

  fromTransitionsStream (fileReader);
  fileReader.close ();

  // delete temporary data maps after constructing statemachine
  // as the mapper data isn't needed at runtime
  mStateNameMapper.clear ();
  mViewNameMapper.clear ();
  
  cout << "mStateList.size (): " << mStateList.size () << endl;
  cout << "mViewList.size (): " << mViewList.size () << endl;
  
  return true;
}

void SimpleLoader::unload ()
{
  // free mViewList
  delete_stl_container <std::vector <View*>, View*> (mViewList);
  
  // free mStateList
  delete_stl_container <std::vector <State*>, State*> (mStateList);
}

void SimpleLoader::fromEventsStream (std::ifstream &in_stream)
{
  int params;

  string line;

  while (getline (in_stream, line))
  {
    cout << endl << line << endl;
    
    addEvent (line);
  }
}

void SimpleLoader::fromViewsStream (std::ifstream &in_stream, const std::string &smDir)
{
  int params;
  string line;
  View *view = NULL;
  bool map = false;
  unsigned int i = 0;

  while (getline (in_stream, line))
  {    
    cout << endl << line << endl;
    
    // parser for 'type' token
    list <string> tokenList;
    stringToken <list <string> > (line, ":", 1, tokenList);
    
    list <string>::const_iterator token = tokenList.begin ();
    if (token == tokenList.end ()) 
      continue;
      
    string type (*token);
    cout << "Type: " << type << endl;
        
    ++token;
    if (token == tokenList.end ()) 
      continue;
    
    string rightSide (*token);
    cout << "rightSide=" << rightSide << endl;

    string name;
    string viewType;
    string fileName;
    string groupName;
    
    if (type == "view")
    {
      map = false;
      
      list <string> tokenList;
      stringToken <list <string> > (rightSide, ":", 4, tokenList);
      
      list <string>::const_iterator token = tokenList.begin ();
      if (token == tokenList.end ()) 
        continue;

      cout << "name=" << *token << endl;
      name = *token;
      mViewNameMapper[name] = i;
      
      ++token;
      if (token == tokenList.end ()) 
        continue;
      
      cout << "ViewType=" << *token << endl;
      viewType = *token;
      
      ++token;
      if (token == tokenList.end ()) 
        continue;
      
      cout << "Filename=" << *token << endl;
      fileName = *token;
      
      ++token;
      // groupName is optional as TextView doesn't need one...
      if (token != tokenList.end ()) 
      {
        cout << "Groupname=" << *token << endl;
        groupName = *token;
      }
      
      if (viewType == "EdjeView")
      {
#if HAVE_EFL
        //view = new EdjeView (smDir + "/" + fileName, groupName);
#else
        cerr << "Error: Compiled sateval without EFL support!" << endl;
        assert (false);
#endif
      }
      else if (viewType == "TextView")
      {
        std::list <std::string> params;
        params.push_back (smDir + "/" + fileName);
        
        view = loadView ("xxx", params);
      }
      else
      {
        cerr << "ViewType '" << viewType << "' not supported!" << endl;
        assert (false);
      }
      ++i;
    }
    else if (type == "map")
    {
      map = true;
      
      list <string> tokenList;
      stringToken <list <string> > (rightSide, "->", 1, tokenList);
      
      list <string>::const_iterator token = tokenList.begin ();
      if (token == tokenList.end ()) 
        continue;
      
      string map_from (*token);
      cout << "map_from=" << map_from << endl;
      
      ++token;
      if (token == tokenList.end ()) 
        continue;
      
      string map_to (*token);
      cout << "map_to=" << map_to << endl;
      
      view->addEventMapping (findMapingEvent (map_from), findMapingEvent (map_to));
    }
    
    if (!map)
    {
      mViewList.push_back (view);
    }
  }
}

void SimpleLoader::fromStatesStream (std::ifstream &in_stream)
{
  int params;
  string line;
  unsigned int i = 0;
  list <list <string> > stringList;

  while (getline (in_stream, line))
  {
    cout << endl << line << endl;
    
    // parser for ':' token
    list <string> tokenList;
    stringToken <list <string> > (line, ":", 0, tokenList);
    stringList.push_back (tokenList);
  }

  // first loop to build up name mapper index...
  for (list <list <string> >::const_iterator sl_it = stringList.begin ();
       sl_it != stringList.end ();
       ++sl_it)
  {
    const list <string> &tokenList = *sl_it;
     ++i;
    
    list <string>::const_iterator token = tokenList.begin ();
    if (token == tokenList.end ()) 
      continue;

    string name (*token);
    cout << "+Name: " << name << endl;
    mStateNameMapper[name] = i;
    ++token;
  }

  // ...now read real data and use name mapper index in second loop...
  for (list <list <string> >::const_iterator sl_it = stringList.begin ();
       sl_it != stringList.end ();
       ++sl_it)
  {
    const list <string> &tokenList = *sl_it;
    ++i;
    
    list <string>::const_iterator token = tokenList.begin ();
    if (token == tokenList.end ()) 
      continue;

    string name (*token);
    cout << "Name: " << name << endl;
    ++token;
    
    string type (*token);
    cout << "Type: " << type << endl;
    
    State *state = NULL;
    
    ++token;
    if (token == tokenList.end ()) 
      continue;
    
    string parent (*token);
    // TODO: better use find() to detect if not found in map
    int parentNum = mStateNameMapper[parent];
    
    CompoundState *parentState = NULL;
    
    if (parentNum != 0) // negative detection of root compound
    {
      parentState = static_cast <CompoundState*> (mStateList[parentNum-1]);
      assert (parentState);
    }
    
    string param2;
    ++token;
    if (token != tokenList.end ())
    {
      param2 = *token;
    }

    cout << "Parent: " << parent << endl;
    
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
      assert (!param2.empty ());
      // TODO: better use find() to detect if not found in map
      int historyNum = mStateNameMapper [param2];
      State *stateTrans = mStateList[historyNum-1];
      HistoryState *historyState = new HistoryState (parentState);
      
      parentState->setHistory (historyState);
      historyState->changeTransition (stateTrans);
      state = historyState;
    }
    else if (type == "DecisionState")
    {
      state = new DecisionState (parentState);
    }
    else if (type == "ViewState")
    {
      assert (!param2.empty ());
      // TODO: better use find() to detect if not found in map
      int viewNum = mViewNameMapper[param2];
      View *view = mViewList[viewNum];
      cout << "View: " << param2 << "," << viewNum << endl;
      state = new ViewState (parentState, *view);
    }
    
    assert (state);
    
    state->setID (i);
    state->setName (name);
    
    addState (state);
  }
}

void SimpleLoader::fromTransitionsStream (std::ifstream &in_stream)
{
  int params;
  string line;

  while (getline (in_stream, line))
  {    
    cout << endl << line << endl;
    
    // parser for '->' token
    list <string> tokenList;
    stringToken <list <string> > (line, "->", 1, tokenList);
    
    list <string>::const_iterator token = tokenList.begin ();
    if (token == tokenList.end ()) 
      continue;
      
    string left (*token);
    
    list <string> tokenList2;
    stringToken <list <string> > (left, ":", 1, tokenList2);
    
    list <string>::const_iterator token2 = tokenList2.begin ();
    if (token2 == tokenList2.end ()) 
      continue;
    
    string from (*token2);
    cout << "from_transition: " << from << endl;
    
    string event; // empty if no event defined (default transition)
    ++token2;
    if (token2 != tokenList2.end ()) 
    {
      event = *token2;
      cout << "event_transition: " << event << endl;
    }
    
    ++token;
    if (token == tokenList.end ()) 
    continue;
    
    string to (*token);
    cout << "to_transition: " << to << endl;

    // TODO: better use find() to detect if not found in map
    int fromStateNum = mStateNameMapper[from];
    int toStateNum = mStateNameMapper[to];
    
    State *fromState = mStateList[fromStateNum-1];
    State *toState = mStateList[toStateNum-1];

    Transition *trans = NULL;
    
    if (event.empty ())
    {
      trans = new Transition (toState);
    }
    else
    {
      trans = new Transition (toState, findMapingEvent (event));
    }    
    
    fromState->addLeaveTransition (*trans);
  }  
}

/*****************************/
/* Plugin needed C functions */
/*****************************/

PLUGIN_EXPORT SimpleLoader *plugin_create ()
{
  return new SimpleLoader ();
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
