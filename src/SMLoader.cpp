#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

/* STD */
#include <iostream>
#include <cassert>

/* Project */
#include "../include/stateval/SMLoader.h"
#include "FileReader.h"
#include "localUtil.h"
#include "stringUtil.h"
#include "../include/stateval/CompoundState.h"
#include "../include/stateval/HistoryState.h"
#include "../include/stateval/DecisionState.h"
#include "../include/stateval/ViewState.h"
#include "../include/stateval/EdjeView.h"

#define MAX_LINE_BUFFER 1024

using namespace std;

SMLoader::SMLoader () :
  eventCounter (0)
{
}

SMLoader::~SMLoader ()
{
  unload ();
}

bool SMLoader::load (const std::string &smDir)
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
  
  //unload ();
  
  cout << "mStateList.size (): " << mStateList.size () << endl;
  cout << "mViewList.size (): " << mViewList.size () << endl;
  
  return true;
}

void SMLoader::unload ()
{
  // free mViewList
  delete_stl_container <std::vector <View*>, View*> (mViewList);
  
  // free mStateList
  delete_stl_container <std::vector <State*>, State*> (mStateList);
}

void SMLoader::fromEventsStream (std::ifstream &in_stream)
{
  int params;

  char buffer[MAX_LINE_BUFFER];
  int v0, v1, v2;

  while (in_stream.getline (buffer, MAX_LINE_BUFFER))
  {
    string line (buffer);
    
    cout << endl << line << endl;
    
    addEvent (line);
  }
}

void SMLoader::fromViewsStream (std::ifstream &in_stream, const std::string &smDir)
{
  int params;

  char buffer[MAX_LINE_BUFFER];
  int v0, v1, v2;
  View *view = NULL;
  bool map = false;
  unsigned int i = 0;

  while (in_stream.getline (buffer, MAX_LINE_BUFFER))
  {
    string line (buffer);
    
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
      if (token == tokenList.end ()) 
        continue;
      
      cout << "Groupname=" << *token << endl;
      groupName = *token;
      
      if (viewType == "EdjeView")
      {
        view = new EdjeView (smDir + "/" + fileName, groupName);
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

void SMLoader::fromStatesStream (std::ifstream &in_stream)
{
  int params;

  char buffer[MAX_LINE_BUFFER];
  int v0, v1, v2;
  unsigned int i = 0;
  list <list <string> > stringList;

  while (in_stream.getline (buffer, MAX_LINE_BUFFER))
  {
    string line (buffer);
    
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

void SMLoader::fromTransitionsStream (std::ifstream &in_stream)
{
  int params;

  char buffer[MAX_LINE_BUFFER];
  int v0, v1, v2;

  while (in_stream.getline (buffer, MAX_LINE_BUFFER))
  {
    string line (buffer);
    
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

void SMLoader::addEvent (const std::string &event)
{
  mEventList[event] = eventCounter;
    
  ++eventCounter;
}

void SMLoader::addState (State *state)
{
  mStateList.push_back (state);
}

void SMLoader::addView (View *view)
{
  mViewList.push_back (view);
}

State *SMLoader::getInitialState ()
{
  return *mStateList.begin ();
}

int SMLoader::findMapingEvent (const std::string &event)
{
    map <string,int>::iterator iter = mEventList.find(event);
    if (iter != mEventList.end())
    {
      int &mapEvent = iter->second;
      //cout << "map event: " << iter->first << " : " << iter->second << endl;
      return mapEvent;
    }

  // TODO: throw exception! as workaround assert
  cerr << "StateMachine::findMapingEvent: try to find not existing event: " << event << endl;
  //assert (false);
  return -1;
}
